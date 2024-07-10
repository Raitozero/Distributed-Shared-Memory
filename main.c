#include "util.h"
#include "hashtable.h"
#include "tcp_udp.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(){
    struct sockaddr_in addr, server_addr_udp, client_addr;
    int udp_socket, master_socket;
    int addr_len;
    int opt = 1;
    msg_t message;

    fd_set readfds;
    init(); //initiate current machine
    initHT(); //initiate hashtable

    //UDP	
	if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1){		//Creating a UDP Socket	 
        perror("Socket");						
        exit(1);
    }

	//current machine address
	server_addr_udp.sin_family = AF_INET;
	server_addr_udp.sin_port = htons(node.udp_port_num);
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
	addr.sin_family = AF_INET;			
  	addr.sin_addr.s_addr = INADDR_ANY;		
	addr.sin_port = htons(node.tcp_port_num);		

	if (bind(master_socket, (struct sockaddr *)&addr, sizeof(addr))<0){
    		perror("TCP bind failed.");
    		exit(EXIT_FAILURE);
  	}
	if (listen(master_socket, 5) < 0){	//maximum of 5 pending connections for the master socket 
        perror("listen");
        exit(EXIT_FAILURE);
  	}

    int addr_len, new_socket;
    while (1) {

   		FD_ZERO(&readfds);//zero fd_set

   		FD_SET(udp_socket, &readfds); //add udp socket to fd_set

   		FD_SET(master_socket, &readfds); //add tcp socket to fd_set

   		FD_SET(0, &readfds);  //add console input to fd_set

        int sel_ret = select(MAX(udp_socket, master_socket)+1, &readfds, NULL, NULL, NULL);//block here to establish new conn or receive new data
        if(sel_ret < 0){
            perror("select failed.");
            exit(EXIT_FAILURE);
        }

        //if tcp master socket activated
        if(FD_ISSET(master_socket, &readfds)){

            addr_len = sizeof(addr);

            if ((new_socket = accept(master_socket, (struct sockaddr *)&addr, &addr_len))<0){ //establish new conn, return a handle: new_socket
                perror("TCP Accept failed.");
                exit(EXIT_FAILURE);
            }
            //addr stores the client's sockaddr
            if (read(new_socket, &message, sizeof(msg_t)) < 0) {
                perror("TCP Receive failed.");
                close(new_socket);
                exit(EXIT_FAILURE);
            }
            else{
                printf("Message Received by node %d\n", curr); //
            }
            close(new_socket);
        }

        //if udp socket,
        if(FD_ISSET(udp_socket, &readfds)){
            msg_t udp_message;
            int udp_ret = recvfrom(udp_socket, &udp_message, sizeof(msg_t), 0,(struct sockaddr *)&client_addr, &addr_len);
            printf("\nUDP packet arrived at [Node %d] from [Node %d, IP: %s, Port: %d]\n",
            curr, udp_message.curr_machine_id, inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
            udp_message.curr_machine_id = curr;
            if(!forThisNode(curr, message)){
                printf("\nMessage is not for this node, forwarding to [Node %d]\n", (curr+1) % NUM_Machine);
                udp_forward(curr+1, message);
            }
            else{// the message is for this node, we establish tcp connection to the request node
                printf("\nProcessing on current Node...\n");
                
				int key = udp_message.key;
                int sock, bytes_recieved;
                int isGet = udp_message.type <= 2? 1:0; // check if the type is a get or put



                struct hostent *host;
                struct sockaddr_in local_addr, server_addr; 
                host = gethostbyname(node.ip_addr);

                if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1){
                    perror("Socket");
                    exit(1);
                }

                //bind to specific tcp port number
                memset(&local_addr, 0, sizeof(local_addr));
                local_addr.sin_family = AF_INET;
                local_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any local address
                local_addr.sin_port = htons(node.tcp_port_num);

                //set up request machine's addr
                memset(&server_addr, 0, sizeof(server_addr));
				server_addr.sin_family = AF_INET;     
                server_addr.sin_port = htons(node.udp_port_num); 
                server_addr.sin_addr = *((struct in_addr *)host->h_addr);
                bzero(&(server_addr.sin_zero),8);

                if (connect(sock, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1){
                    perror("Connect");
                    exit(1);
                }
                if(isGet){ //if it is a get request, fetch the value from HT, then send it back to the request node
                    //initiate the reply
                    msg_t* reply = malloc(sizeof(msg_t));
                    reply->key = udp_message.key;
                    reply->type = GET_REPLY;
                    reply->curr_machine_id = curr;
                    reply->original_machine_id = curr;
                    reply->original_port_num = node.tcp_port_num;

                    int value = getFromHT(key);
                    if(value != -1) reply->value = value;
                    else reply->NotFound = 1;
                    send(sock, &(*reply), sizeof(msg_t), 0); //Send the fetched value
                    printf("\nReply successfullly send from [Node %d]---->[Node %d]\n", curr, udp_message.original_machine_id);
                }
                else{//if it is a put request, add it to hashtable
                    msg_t* put_message = malloc(sizeof(msg_t));
                    bytes_recieved=recv(sock, &put_message, sizeof(msg_t), 0);	//Receiving Put value
              				
					printf("\nValue Received (Via TCP) From [Node %d] to [Node %d]", udp_message.original_machine_id, curr);
                    if(putToHT(put_message->key, put_message->value)){
                        printf("\nValue Added to Hashtable.\n");
                    }
				}	
                close(sock);
                fflush(stdout);
            }
        }
    return 0;
}


