#ifndef LOAD_BALANCER_H
#define LOAD_BALANCER_H

#include "server.h"
#include <json-c/json.h>

// Load Balancing Rule Structure
typedef struct {
    char path[256];
    char algorithm[64];
    Server **servers;
    int server_count;
} LoadBalancingRule;

// Load Balancer Structure
typedef struct {
    LoadBalancingRule *rules;
    int rule_count;
} LoadBalancer;

// Initialize Load Balancer
int initialize_load_balancer(const char *config_file, LoadBalancer *lb);

// Select Backend Server based on Weighted Least Connections
Server* select_backend_server(LoadBalancer *lb, const char *request_path);

// Add a new Load Balancing Rule
int add_load_balancing_rule(LoadBalancer *lb, LoadBalancingRule rule);

// Remove a Load Balancing Rule
int remove_load_balancing_rule(LoadBalancer *lb, const char *path);

// Cleanup Load Balancer
void cleanup_load_balancer(LoadBalancer *lb);

#endif // LOAD_BALANCER_H
