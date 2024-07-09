#include <stdio.h>
#include <stdlib.h>

#define NUM_Machine 6
#define DEFAULT_IP "127.0.0.1"

volatile int curr = 0;


typedef struct msg{
    int type;
    int key;
    int value;
    unsigned int addr;
    unsigned int port_num;
} msg_t;

typedef struct machine{
    char* ip_addr;
    unsigned int tcp_port_num;
    unsigned int udp_port_num;
} machine;

machine nodes[NUM_Machine];


void init(machine nodes[NUM_Machine]);






//request

int getOrPut(char rec_buff[]);
int extractKeyFromGet(char request[]);
int extractValueFromPut(char request[]);