#include "tcp_udp.h"
#include "hashtable.h"

#include <sys/socket.h>
#include <netdb.h>

void udp_forward(int next_node, msg_t message){

	int sock;
	struct sockaddr_in server_addr;
	next_node = hash(next_node);

	struct hostent *host;
	host = (struct hostent *) gethostbyname(node.ip_addr);
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
		perror("socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(node.udp_port_num);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

    //send to next node the message
	sendto(sock, &message, sizeof(msg_t), 0,(struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    
	printf("\nSuccess: UDP Forwarded From [Node %d] to [Node %d]\n", curr, next_node);
	close(sock);
} 