/**
 * @file	hamster_server.c
 * @brief	Remote Hamsterverwaltungsprogramm
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>

#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "hamsterlib.h"

#include "hamsterrpc.h"
#include "structs.h"
#include "payloadHandler.h"
#include "headerHandler.h"
#include "replyWriter.h"

//some hamsterprotocol specifications

/*
 * @brief Kurze Hilfe ausgeben
 */
static void rtfm(char* argv[])
{
	printf("Usage: %s {<Option>} <param1> \n", argv[0]);
	printf("Function: Hamster management server\n");
	printf("Optionen:\n");
	printf("     -p {<port>}                    - port to run the server\n");
	printf("     -h {<IP address>}              - IP address to run the server on (default: 127.0.0.1\n");
}



/**
 * @brief Main program
**/
int main(int argc, char* argv[])
{

    /*
    Hallo armer Mensch der das hier kontrollieren muss.
    Vieles hier drin ist bisschen Quatsch geworden, weil ich erst spät gemerkt hab was ich
    eigentlich zu tun habe. Ich hoffe, man kommt trotzdem einigermaßen im Code zurecht.

    Liebe Grüße und nochmal Entschuldigung für das Wirrwarr,
    Tim
    */
    if(argc<2){
		rtfm(argv);
	}

    bool run = true;
	int portnumber = 2323;    

	int server_fd, new_socket;
    struct sockaddr_in address;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    int opt = 1;
    int addrlen = sizeof(address);

    //check parameters with which program hamster_server was called
    if(argc == 3 || argc == 5){
        for(int i = 0; i<argc; i++){
            if(strcmp(argv[i], "-p")==0){
                portnumber = strtol(argv[i+1], NULL, 10);
                if(portnumber < 1024){
                    printf("please use a pornumber >1024\ngoing back to default 2323...\n");
                    portnumber = 2323;
                    rtfm(argv);
                }
            }
            else if(strcmp(argv[i], "-h")==0){
                printf("read ipAdress: %s\n", argv[i+1]);
                if (inet_pton(AF_INET, argv[i+1], &(address.sin_addr)) <= 0){
                    printf("invalid IP adress\nreverting to default\n");
                    inet_pton(AF_INET, "127.0.0.1", &(address.sin_addr));
                }
            }
        }
    }
    else{
        printf("too many arguments\ndefault values, server on 127.0.0.1:2323\n");
        rtfm(argv);
    }

    header receiveHeader;
    header sendHeader;
    uint8_t headerStream[8];

    //char *hello = "Hello from server";  
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_port = htons( portnumber );
       
    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(run){
        

        printf("server in listening loop\n");

        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        printf("accept done!\n");
        /*
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        */

        while(true){

            //RPC Header lesen
            if(read( new_socket , headerStream, 8)==0){
                printf("end of message reached\n");
                break;    
            } else {
                printf("new header read\n");
                handleHeader(headerStream, &receiveHeader, &sendHeader);
                for(int i=0; i<8; i++){
                    printf("%#08X ", headerStream[i]);
                    if((i+1)%4 == 0)
                        printf("\n");
                }
            }

            uint8_t payloadStream[receiveHeader.pay_size];
            printf("reading %d bytes from payloadStream\n", receiveHeader.pay_size);
            //Payload lesen
            read(new_socket, payloadStream, receiveHeader.pay_size);

            //buffer für reply
            uint8_t replyBuffer[sendHeader.pay_size +8];
            int retValue = handlePayload(payloadStream, replyBuffer, &receiveHeader, &sendHeader);
            printf("return value after handling payload: %d", retValue);
            if(retValue < 0){
                uint8_t errorBuffer[12];
                constructErrorMsg(errorBuffer, &sendHeader, &retValue);

                printf("sending %d bytes\n", sendHeader.pay_size);
                printf("hexdump of sendbuffer:\n");
                for(int i=0; i<sendHeader.pay_size+8; i++){
                    printf("%#08X ", errorBuffer[i]);
                    if((i+1)%4 == 0)
                        printf("\n");
                }

                send(new_socket , errorBuffer , sendHeader.pay_size+8 , 0 );
                
            }
            else{
                printf("sending %d bytes\n", sendHeader.pay_size);
                printf("hexdump of sendbuffer:\n");
            
                for(int i=0; i<sendHeader.pay_size+8; i++){
                    printf("%#08X ", replyBuffer[i]);
                    if((i+1)%4 == 0)
                        printf("\n");
                }
                //reply senden
                send(new_socket , replyBuffer , sendHeader.pay_size + 8 , 0 );
            }
            printf("\nawaiting new instructions...\n\n");
        }

        close(new_socket);
    }

	return 0;
}

