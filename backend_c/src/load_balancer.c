#include "load_balancer.h"
#include "utils.h"
#include <json-c/json.h>
#include <string.h>
#include <stdlib.h>

int initialize_load_balancer(const char *config_file, LoadBalancer *lb) {
    FILE *fp = fopen(config_file, "r");
    if (!fp) {
        log_message(LOG_ERROR, "Failed to open config file: %s", config_file);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);

    char *json_str = (char*)malloc(fsize + 1);
    fread(json_str, 1, fsize, fp);
    json_str[fsize] = 0;
    fclose(fp);

    struct json_object *parsed_json;
    parsed_json = json_tokener_parse(json_str);
    free(json_str);

    if (!parsed_json) {
        log_message(LOG_ERROR, "Failed to parse JSON config.");
        return -1;
    }

    struct json_object *rules_array;
    if (!json_object_object_get_ex(parsed_json, "rules", &rules_array)) {
        log_message(LOG_ERROR, "No 'rules' found in config.");
        json_object_put(parsed_json);
        return -1;
    }

    int rule_count = json_object_array_length(rules_array);
    lb->rules = (LoadBalancingRule*)malloc(sizeof(LoadBalancingRule) * rule_count);
    lb->rule_count = rule_count;

    for (int i = 0; i < rule_count; i++) {
        struct json_object *rule_obj = json_object_array_get_idx(rules_array, i);
        struct json_object *path_obj, *algorithm_obj, *servers_array;

        json_object_object_get_ex(rule_obj, "path", &path_obj);
        json_object_object_get_ex(rule_obj, "algorithm", &algorithm_obj);
        json_object_object_get_ex(rule_obj, "servers", &servers_array);

        strncpy(lb->rules[i].path, json_object_get_string(path_obj), sizeof(lb->rules[i].path)-1);
        strncpy(lb->rules[i].algorithm, json_object_get_string(algorithm_obj), sizeof(lb->rules[i].algorithm)-1);

        int server_count = json_object_array_length(servers_array);
        lb->rules[i].servers = (Server**)malloc(sizeof(Server*) * server_count);
        lb->rules[i].server_count = server_count;

        for (int j = 0; j < server_count; j++) {
            struct json_object *server_obj = json_object_array_get_idx(servers_array, j);
            struct json_object *id_obj, *ip_obj, *port_obj, *weight_obj;

            json_object_object_get_ex(server_obj, "id", &id_obj);
            json_object_object_get_ex(server_obj, "ip", &ip_obj);
            json_object_object_get_ex(server_obj, "port", &port_obj);
            json_object_object_get_ex(server_obj, "weight", &weight_obj);

            char *id = (char*)json_object_get_string(id_obj);
            char *ip = (char*)json_object_get_string(ip_obj);
            int port = json_object_get_int(port_obj);
            int weight = json_object_get_int(weight_obj);

            lb->rules[i].servers[j] = initialize_server(id, ip, port, weight);
            log_message(LOG_INFO, "Initialized server %s at %s:%d with weight %d", id, ip, port, weight);
        }
    }

    json_object_put(parsed_json);
    log_message(LOG_INFO, "Load balancer initialized with %d rules.", rule_count);
    return 0;
}

Server* select_backend_server(LoadBalancer *lb, const char *request_path) {
    for (int i = 0; i < lb->rule_count; i++) {
        if (strncmp(request_path, lb->rules[i].path, strlen(lb->rules[i].path)) == 0) {
            // Implement Weighted Least Connections
            Server *selected = NULL;
            int min_score = __INT32_MAX__;

            for (int j = 0; j < lb->rules[i].server_count; j++) {
                Server *srv = lb->rules[i].servers[j];
                int score = srv->active_connections * (1.0 / srv->weight);
                if (score < min_score) {
                    min_score = score;
                    selected = srv;
                }
            }

            if (selected) {
                increment_active_connections(selected);
                log_message(LOG_INFO, "Selected server %s for path %s", selected->id, request_path);
            }

            return selected;
        }
    }

    log_message(LOG_WARNING, "No matching rule found for path %s", request_path);
    return NULL;
}

int add_load_balancing_rule(LoadBalancer *lb, LoadBalancingRule rule) {
    lb->rules = realloc(lb->rules, sizeof(LoadBalancingRule) * (lb->rule_count + 1));
    if (!lb->rules) return -1;
    lb->rules[lb->rule_count] = rule;
    lb->rule_count += 1;
    log_message(LOG_INFO, "Added new load balancing rule for path %s", rule.path);
    return 0;
}

int remove_load_balancing_rule(LoadBalancer *lb, const char *path) {
    int found = -1;
    for (int i = 0; i < lb->rule_count; i++) {
        if (strcmp(lb->rules[i].path, path) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) return -1;

    // Free servers
    for (int j = 0; j < lb->rules[found].server_count; j++) {
        free(lb->rules[found].servers[j]);
    }
    free(lb->rules[found].servers);

    // Shift rules
    for (int i = found; i < lb->rule_count - 1; i++) {
        lb->rules[i] = lb->rules[i + 1];
    }
    lb->rule_count -= 1;
    lb->rules = realloc(lb->rules, sizeof(LoadBalancingRule) * lb->rule_count);
    log_message(LOG_INFO, "Removed load balancing rule for path %s", path);
    return 0;
}

void cleanup_load_balancer(LoadBalancer *lb) {
    for (int i = 0; i < lb->rule_count; i++) {
        for (int j = 0; j < lb->rules[i].server_count; j++) {
            free(lb->rules[i].servers[j]);
        }
        free(lb->rules[i].servers);
    }
    free(lb->rules);
    log_message(LOG_INFO, "Load balancer cleaned up.");
}
