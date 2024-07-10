#include "util.h"
#include <stdio.h>
#include <stdlib.h>


void init(){
    srand(time(NULL));
    int random_num = rand();
    node.ip_addr = DEFAULT_IP;
    node.tcp_port_num = random_num;
    node.tcp_port_num = random_num+ 1;
}

int forThisNode(int curr, msg_t message){
	return (message.key % NUM_Machine == curr)? 1 : 0;
}





