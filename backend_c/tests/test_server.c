#include "../src/server.h"
#include <assert.h>
#include <stdio.h>

void test_server_initialization() {
    Server *srv = initialize_server("server1", "127.0.0.1", 8081, 2);
    assert(srv != NULL);
    assert(strcmp(srv->id, "server1") == 0);
    assert(strcmp(srv->ip, "127.0.0.1") == 0);
    assert(srv->port == 8081);
    assert(srv->weight == 2);
    assert(srv->active_connections == 0);

    increment_active_connections(srv);
    assert(srv->active_connections == 1);

    decrement_active_connections(srv);
    assert(srv->active_connections == 0);

    free(srv);
    printf("test_server_initialization passed.\n");
}

int main() {
    test_server_initialization();
    return 0;
}
