#include "server.h"
#include <stdlib.h>
#include <string.h>

Server* initialize_server(const char *id, const char *ip, int port, int weight) {
    Server *server = (Server*)malloc(sizeof(Server));
    if (!server) return NULL;
    strncpy(server->id, id, sizeof(server->id) - 1);
    strncpy(server->ip, ip, sizeof(server->ip) - 1);
    server->port = port;
    server->weight = weight;
    server->active_connections = 0;
    return server;
}

void increment_active_connections(Server *server) {
    if (server) {
        server->active_connections += 1;
    }
}

void decrement_active_connections(Server *server) {
    if (server && server->active_connections > 0) {
        server->active_connections -= 1;
    }
}
