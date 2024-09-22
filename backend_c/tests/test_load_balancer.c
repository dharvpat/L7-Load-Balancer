#include "../src/load_balancer.h"
#include "../src/server.h"
#include "../src/utils.h"
#include <assert.h>
#include <stdio.h>

void test_initialize_load_balancer() {
    LoadBalancer lb;
    int ret = initialize_load_balancer("../config/load_balancer_config.json", &lb);
    assert(ret == 0);
    assert(lb.rule_count == 1);
    assert(strcmp(lb.rules[0].path, "/api/*") == 0);
    assert(strcmp(lb.rules[0].algorithm, "weighted_least_connections") == 0);
    assert(lb.rules[0].server_count == 2);
    cleanup_load_balancer(&lb);
    printf("test_initialize_load_balancer passed.\n");
}

int main() {
    // Initialize Logger for tests
    initialize_logger("logs/test_load_balancer.log", LOG_INFO);

    test_initialize_load_balancer();

    cleanup_logger();
    return 0;
}
