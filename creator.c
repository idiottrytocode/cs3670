
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXSIZE 1000000

struct request{
	char who; 
	int id[4];
	int status; 
	int term; 
	int msgSize;
	char body[MAXSIZE]; 
};

struct response{
	int term;
	int msgSize; 
	char body[MAXSIZE];
}


int main(void)
{
	/*----------------------*/
	/* section 0 prepare creator log and queue*/
	// assume job size smaller than MAXSIZE, no need subdivide 
	int log = 1; 
	char* job = "13 IAMJOBAREYOUSTEVE\0";
	char* array[log];
	array[0]=job;
	/*----------------------*/
	/* section 1 prepare response*/
	struct request req;
	struct response res; 
	memset(server_message, '\0', sizeof(server_message));
	memset(client_message, '\0', sizeof(client_message));
	// assume dummy for network testing 
	res.term =0;
	res.msgSize=1;	
	res.body[0]=13;

	/*----------------------*/
	/* section 2 prepare socket*/

	int socket_desc, client_sock, client_size;
	struct sockaddr_in server_addr, client_addr;

	socket_desc = socket(AF_INET, SOCK_STREAM, 0);

	if(socket_desc < 0){
		printf("Error while creating socket\n");
		return -1;
	}
	printf("Socket created successfully\n");

	// Set port and IP:
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(2000);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// Bind to the set port and IP:
	if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
		printf("Couldn't bind to the port\n");
		return -1;
	}
	printf("Done with binding\n");

	// Listen for clients:
	if(listen(socket_desc, 1) < 0){
		printf("Error while listening\n");
		return -1;
	}
	printf("\nListening for incoming connections.....\n");

	// Accept an incoming connection:
	client_size = sizeof(client_addr);
	client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);

	if (client_sock < 0){
		printf("Can't accept\n");
		return -1;
	}
	printf("Client connected at IP: %s and port: %i\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	// Receive client's message:
	if (recv(client_sock, req, sizeof(struct request), 0) < 0){
		printf("Couldn't receive\n");
		return -1;
	}
	
	// checking incoming msg is from seeker and identify status 
	// if request status ==1 then send back job type in response body
	if(req.status == 1){

		strcpy(req.body,job);		
		if (send(client_sock, res, sizeof(res), 0) < 0){
			printf("Can't send\n");
			return -1;
		}			
	}

	// Respond to seekers status 2 acceping request 
	if (recv(client_sock, req, sizeof(struct request), 0) < 0){
		printf("Couldn't receive\n");
		return -1;
	}
	if(req.status == 2){

		if (send(client_sock, res, sizeof(res), 0) < 0){
			printf("Can't send\n");
			return -1;
		}			
		// pop queue ommited for this version 

	}

	// seeker send success report close sned instruction to close connection 
	// reduce log  
	if (recv(client_sock, req, sizeof(struct request), 0) < 0){
		printf("Couldn't receive\n");
		return -1;
	}
	if(req.status == 4){

		res.term =1; 
		if (send(client_sock, res, sizeof(res), 0) < 0){
			printf("Can't send\n");
			return -1;
		}			
		log--;
	}


	// a grand loop here will dealt with all other cases if log is stll postive 

	// Closing the socket:
	if(log<=0){
		close(client_sock);
		close(socket_desc);
	}
	
	
	return 0;
}

