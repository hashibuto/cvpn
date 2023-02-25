#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * get_command(int argc, char *argv[]) {
    if (argc > 1) {
        return argv[1];
    }

    return NULL;
}

struct ServeCommandNs {
    char iface[20];
    char network[20];
    int port;
};

struct CallCommandNs {

};

// parse_serve parses the serve command
struct ServeCommandNs *parse_serve(int argc, char *argv[]) {
    struct ServeCommandNs *sc_ns = malloc(sizeof(struct ServeCommandNs));
    memset(sc_ns, 0, sizeof(struct ServeCommandNs));
    char *cur_option = NULL;
    for (int i = 0; i < argc; i++) {
        char *cur_arg = argv[i];
        if (strlen(cur_arg) > 1 && cur_arg[0] == '-') {
            cur_option = cur_arg;
        } else {
            if (cur_option != NULL) {
                if (strcmp(cur_option, "-i") == 0 || strcmp(cur_option, "--interface") == 0) {
                    strncpy(sc_ns->iface, cur_arg, sizeof(sc_ns->iface));
                } else if (strcmp(cur_option, "-n") == 0 || strcmp(cur_option, "--network") == 0) {
                    strncpy(sc_ns->network, cur_arg, sizeof(sc_ns->network));
                } else if (strcmp(cur_option, "-p") == 0 || strcmp(cur_option, "--port") == 0) {
                    sc_ns->port = atoi(cur_arg);
                }
            }

            cur_option = NULL;
        }
    }

    if (strlen(sc_ns->iface) == 0 || strlen(sc_ns->network) == 0 || sc_ns->port <= 0) {
        return NULL;
    }

    return sc_ns;
}

// parse_connect parses the client connect command
struct CallCommandNs *parse_connect(int argc, char *argv[]) {

}

int main(int argc, char *argv[]) {
    char *command = get_command(argc, argv);
    if (command != NULL) {
        if (strcmp(command, "serve") == 0) {
            struct ServeCommandNs *serve_command = parse_serve(argc, argv);
            if (serve_command != NULL) {
                return 0;
            }

            printf("cvpn serve [...options]\n");
            printf("options:\n");
            printf("    -i/--interface              name of the tap/tun interface (required)\n");
            printf("    -n/--network                network CIDR range for the VPN network (required)\n");
            printf("    -p/--port                   listen port (required)\n");
            return 1;
        }
    }

    printf("cvpn <command> [...options]\n");
    printf("commands:\n");
    printf("    serve\n");
    printf("    connect\n");
    return 1;
}