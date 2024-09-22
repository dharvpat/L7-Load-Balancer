#include <microhttpd.h>
#include "load_balancer.h"
#include "utils.h"
#include "server.h"
#include <string.h>
#include <stdlib.h>

// Global Load Balancer Instance
LoadBalancer lb;

// Callback to handle incoming API requests
static int handle_request(void *cls, struct MHD_Connection *connection,
                          const char *url, const char *method,
                          const char *version, const char *upload_data,
                          size_t *upload_data_size, void **con_cls) {
    // Only handle POST and GET methods
    if (strcmp(method, "GET") && strcmp(method, "POST") && strcmp(method, "DELETE") && strcmp(method, "PUT")) {
        const char *error = "{\"error\": \"Method not allowed\"}";
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error), (void*)error, MHD_RESPMEM_PERSISTENT);
        int ret = MHD_queue_response(connection, MHD_HTTP_METHOD_NOT_ALLOWED, response);
        MHD_destroy_response(response);
        return ret;
    }

    // Simple Routing based on URL
    if (strcmp(url, "/api/rules") == 0 && strcmp(method, "GET") == 0) {
        // Retrieve all rules
        struct json_object *jobj = json_object_new_object();
        struct json_object *rules_array = json_object_new_array();

        for (int i = 0; i < lb.rule_count; i++) {
            struct json_object *rule_obj = json_object_new_object();
            json_object_object_add(rule_obj, "path", json_object_new_string(lb.rules[i].path));
            json_object_object_add(rule_obj, "algorithm", json_object_new_string(lb.rules[i].algorithm));

            struct json_object *servers_array = json_object_new_array();
            for (int j = 0; j < lb.rules[i].server_count; j++) {
                struct json_object *server_obj = json_object_new_object();
                json_object_object_add(server_obj, "id", json_object_new_string(lb.rules[i].servers[j]->id));
                json_object_object_add(server_obj, "ip", json_object_new_string(lb.rules[i].servers[j]->ip));
                json_object_object_add(server_obj, "port", json_object_new_int(lb.rules[i].servers[j]->port));
                json_object_object_add(server_obj, "weight", json_object_new_int(lb.rules[i].servers[j]->weight));
                json_object_array_add(servers_array, server_obj);
            }
            json_object_object_add(rule_obj, "servers", servers_array);
            json_object_array_add(rules_array, rule_obj);
        }

        json_object_object_add(jobj, "rules", rules_array);
        const char *response_str = json_object_to_json_string(jobj);
        struct MHD_Response *response = MHD_create_response_from_buffer(strlen(response_str),
                                                                        (void*)response_str,
                                                                        MHD_RESPMEM_MUST_COPY);
        MHD_add_response_header(response, "Content-Type", "application/json");
        int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
        MHD_destroy_response(response);
        json_object_put(jobj);
        return ret;
    }
    else if (strcmp(url, "/api/rules") == 0 && strcmp(method, "POST") == 0) {
        // Add a new rule
        static char *post_data = NULL;
        static size_t size = 0;

        if (*upload_data_size != 0) {
            post_data = realloc(post_data, size + *upload_data_size + 1);
            memcpy(post_data + size, upload_data, *upload_data_size);
            size += *upload_data_size;
            post_data[size] = '\0';
            *upload_data_size = 0;
            return MHD_YES;
        }
        else {
            // Parse JSON and add the rule
            struct json_object *jobj = json_tokener_parse(post_data);
            free(post_data);
            post_data = NULL;
            size = 0;

            if (!jobj) {
                const char *error = "{\"error\": \"Invalid JSON\"}";
                struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error), (void*)error, MHD_RESPMEM_PERSISTENT);
                int ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
                MHD_destroy_response(response);
                return ret;
            }

            struct json_object *path_obj, *algorithm_obj, *servers_array;
            if (!json_object_object_get_ex(jobj, "path", &path_obj) ||
                !json_object_object_get_ex(jobj, "algorithm", &algorithm_obj) ||
                !json_object_object_get_ex(jobj, "servers", &servers_array)) {
                const char *error = "{\"error\": \"Missing fields in JSON\"}";
                struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error), (void*)error, MHD_RESPMEM_PERSISTENT);
                int ret = MHD_queue_response(connection, MHD_HTTP_BAD_REQUEST, response);
                MHD_destroy_response(response);
                json_object_put(jobj);
                return ret;
            }

            LoadBalancingRule new_rule;
            strncpy(new_rule.path, json_object_get_string(path_obj), sizeof(new_rule.path)-1);
            strncpy(new_rule.algorithm, json_object_get_string(algorithm_obj), sizeof(new_rule.algorithm)-1);

            int server_count = json_object_array_length(servers_array);
            new_rule.servers = (Server**)malloc(sizeof(Server*) * server_count);
            new_rule.server_count = server_count;

            for (int i = 0; i < server_count; i++) {
                struct json_object *server_obj = json_object_array_get_idx(servers_array, i);
                struct json_object *id_obj, *ip_obj, *port_obj, *weight_obj;

                json_object_object_get_ex(server_obj, "id", &id_obj);
                json_object_object_get_ex(server_obj, "ip", &ip_obj);
                json_object_object_get_ex(server_obj, "port", &port_obj);
                json_object_object_get_ex(server_obj, "weight", &weight_obj);

                char *id = (char*)json_object_get_string(id_obj);
                char *ip = (char*)json_object_get_string(ip_obj);
                int port = json_object_get_int(port_obj);
                int weight = json_object_get_int(weight_obj);

                new_rule.servers[i] = initialize_server(id, ip, port, weight);
            }

            if (add_load_balancing_rule(&lb, new_rule) != 0) {
                const char *error = "{\"error\": \"Failed to add rule\"}";
                struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error), (void*)error, MHD_RESPMEM_PERSISTENT);
                int ret = MHD_queue_response(connection, MHD_HTTP_INTERNAL_SERVER_ERROR, response);
                MHD_destroy_response(response);
                json_object_put(jobj);
                return ret;
            }

            const char *success = "{\"message\": \"Rule added successfully\"}";
            struct MHD_Response *response = MHD_create_response_from_buffer(strlen(success),
                                                                            (void*)success,
                                                                            MHD_RESPMEM_PERSISTENT);
            int ret = MHD_queue_response(connection, MHD_HTTP_CREATED, response);
            MHD_destroy_response(response);
            json_object_put(jobj);
            return ret;
        }
    }
    else if (strncmp(url, "/api/rules/", 11) == 0) {
        // Handle GET, PUT, DELETE for specific rule
        const char *rule_path = url + 11; // Extract the path after /api/rules/
        if (strcmp(method, "DELETE") == 0) {
            if (remove_load_balancing_rule(&lb, rule_path) != 0) {
                const char *error = "{\"error\": \"Rule not found\"}";
                struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error), (void*)error, MHD_RESPMEM_PERSISTENT);
                int ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
                MHD_destroy_response(response);
                return ret;
            }

            const char *success = "{\"message\": \"Rule deleted successfully\"}";
            struct MHD_Response *response = MHD_create_response_from_buffer(strlen(success),
                                                                            (void*)success,
                                                                            MHD_RESPMEM_PERSISTENT);
            int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
            MHD_destroy_response(response);
            return ret;
        }
        // Implement GET and PUT as needed
    }
    // Implement other routes as needed

    const char *error = "{\"error\": \"Not Found\"}";
    struct MHD_Response *response = MHD_create_response_from_buffer(strlen(error), (void*)error, MHD_RESPMEM_PERSISTENT);
    int ret = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
    MHD_destroy_response(response);
    return ret;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <config_file>\n", argv[0]);
        return 1;
    }

    // Initialize Logger
    if (initialize_logger("logs/load_balancer.log", LOG_INFO) != 0) {
        fprintf(stderr, "Failed to initialize logger.\n");
        return 1;
    }

    // Initialize Load Balancer
    if (initialize_load_balancer(argv[1], &lb) != 0) {
        log_message(LOG_ERROR, "Failed to initialize load balancer.");
        cleanup_logger();
        return 1;
    }

    // Start HTTP Server
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, 8888, NULL, NULL,
                              &handle_request, NULL, MHD_OPTION_END);
    if (NULL == daemon) {
        log_message(LOG_ERROR, "Failed to start HTTP server.");
        cleanup_load_balancer(&lb);
        cleanup_logger();
        return 1;
    }

    log_message(LOG_INFO, "Load Balancer HTTP server started on port 8888.");

    // Run indefinitely
    getchar();

    // Shutdown procedures
    MHD_stop_daemon(daemon);
    cleanup_load_balancer(&lb);
    cleanup_logger();

    return 0;
}
