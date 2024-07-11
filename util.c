#include "util.h"
#include <stdio.h>
#include <stdlib.h>


void init(){
    srand(time(NULL));
    int random_num = rand();
    node.ip_addr = DEFAULT_IP;
    node.tcp_port_num = random_num;
    node.udp_port_num = random_num+ 1;
}

void inputMsg(msg_t* message){
    printf("\nPut or Get? 0 for Put, 1 for Get.\n");
    scanf("%d", &message->type);
    printf("key?\n");
    scanf("%d", &message->key);
    printf("value?\n");
    scanf("%d", &message->value);
    message->curr_machine_id = curr;
    message->original_machine_id = curr;
    message->original_port_num = node.tcp_port_num;
}


int forThisNode(int curr, msg_t message){
	return (message.key % NUM_Machine == curr)? 1 : 0;
}





