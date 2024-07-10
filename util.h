#include <stdio.h>
#include <stdlib.h>

#define NUM_Machine 6
#define DEFAULT_IP "127.0.0.1"

volatile int curr = 0;

typedef struct machine{
    char* ip_addr;
    unsigned int tcp_port_num;
    unsigned int udp_port_num;
} machine;

machine node;

typedef struct msg{
    int type;
    int key;
    int value;
    int NotFound;
    int curr_machine_id;
    int original_machine_id;
    int original_port_num;
} msg_t;


enum type{
    GET,
    GET_FORWARD,
    GET_REPLY,
    PUT_FORWARD,
    PUT_REPLY,
    WHAT_VALUE
};


//functions
void init();
int forThisNode(int curr, msg_t message);