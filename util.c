#include "util.h"
#include <stdio.h>
#include <stdlib.h>


void init(machine nodes[NUM_Machine]){
    srand(time(NULL));
    int random_num = rand();
    for(int i = 0; i < NUM_Machine; i++){
        nodes[i].ip_addr = DEFAULT_IP;
        nodes[i].tcp_port_num = random_num * i;
        nodes[i].tcp_port_num = random_num * i + 1;
    }
}






