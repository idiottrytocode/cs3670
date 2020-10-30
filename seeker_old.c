// 1. creatre open socket and link with server 
// 2. identify myself as boss or worker by an int 
// 3. if I am a worker send out a worker id and job selection 
// 4. else if I am a boss send a boss id 
// 5. wait for response
//
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<time.h>
#include <netinet/in.h>
#include<string.h>
#include <arpa/inet.h>


void errMsg(int num, char* str){
	perror(str);
	exit(num);
}

#define PORT 4999 

int main(int argc, char* argv[]){
	// seed srand by time 
	time_t t; 
	srand((unsigned) time(&t));
	// take 1 or 2 argument in 
	// 1 for identify boss or worker 
	// 2 for identify selection mnumber 
	int sfd; 

	if(argc < 1 )errMsg(1, "arg count invalid");

	// pre set ip to bounce back ip 
	// prepare socket 

	sfd = socket(AF_INET, SOCK_STREAM, 0);	
	if(sfd == -1) errMsg(2,"socket creation"); 

	// set addr 
	struct sockaddr_in addr; 
	memset(&addr, 0 , sizeof(struct sockaddr_in)); 
	addr.sin_family = AF_INET; 
	addr.sin_port = PORT;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	// prepare addr and binding it 
	int ret; 
	ret = connect(sfd, (struct sockaddr*)&addr,sizeof(addr)); 
	if(ret==-1)errMsg(4,"connction socket"); 


	// create payload 
	int id = rand()%1000; 	
	char sid [4]; 
	sprintf(sid, "%d", id);

	// str is the msg actually being sent 
	// first msg
	char str[10] = argv[1] + " " + sid ;



	// send msg 
	ret = send(sfd, str, 10, 0); 
	if(ret ==-1) errMsg(5, "send socket"); 

	char list[1000]; 
	ret = recv(sfd,list,1000,0 );
	if(ret ==-1) errMsg(6, "recv socket"); 

	// parse the recieved msg 
	//@ lin you should be able to get this part done 
	//create a var called cntr which store the first int type 

	// send second msg 

	// wait for final reply 





}
