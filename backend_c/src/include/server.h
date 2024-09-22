#ifndef SERVER_H
#define SERVER_H

typedef struct {
    char id[64];
    char ip[16];
    int port;
    int weight;
    int active_connections;
} Server;

// Initialize Server
Server* initialize_server(const char *id, const char *ip, int port, int weight);

// Increment Active Connections
void increment_active_connections(Server *server);

// Decrement Active Connections
void decrement_active_connections(Server *server);

#endif // SERVER_H
