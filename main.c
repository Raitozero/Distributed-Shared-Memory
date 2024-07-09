#include "util.h"
#include "hashtable.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>



int main(){
    struct sockaddr_in address, server_addr_udp, client_addr;
    int udp_socket, master_socket;
    int addr_len;
    int opt = 1;

    fd_set readfds;
    initHT();

    //UDP	
	if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){		//Creating a UDP Socket	 
        perror("Socket");						
        exit(1);
    }

	//current machine address
	server_addr_udp.sin_family = AF_INET;
	server_addr_udp.sin_port = htons(nodes[curr].udp_port_num);
	server_addr_udp.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr_udp.sin_zero),8);	//bzero() sets all values in a buffer to zero.

	if (bind(master_socket,(struct sockaddr *)&server_addr_udp, sizeof(struct sockaddr)) == -1){		
            perror("Bind");	
            exit(1);
    }
    addr_len = sizeof(struct sockaddr);



    //TCP
	if ((master_socket = socket(AF_INET,SOCK_STREAM,0))==0) 		//Create TCP Socket
	{				
		perror("Create master_socket");
    		exit(EXIT_FAILURE);
  	}

  	if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))<0) // allow multiple connections
	{
    		perror("setsockopt");
    		exit(EXIT_FAILURE);
  	}

	//current node address for tcp
	address.sin_family = AF_INET;			
  	address.sin_addr.s_addr = INADDR_ANY;		
	address.sin_port = htons(nodes[curr].tcp_port_num);		

	if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0){
    		perror("bind");
    		exit(EXIT_FAILURE);
  	}
	if (listen(master_socket, 5)<0){	//maximum of 5 pending connections for the master socket 
        perror("listen");
        exit(EXIT_FAILURE);
  	}

    


    return 0;
}


