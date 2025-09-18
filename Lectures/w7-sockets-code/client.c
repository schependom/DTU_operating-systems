#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define ADDRESS   "127.0.0.1"
#define PORT      9876

char msg[2048];
char rcv_buf[2048];

int main(void) {

	int sock; // My socket

	int len;

	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET; // IPv4 Address
	inet_pton(AF_INET, ADDRESS, &server_address.sin_addr);
	server_address.sin_port = htons(PORT); // Set port number


	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) { // Open a UDP Socket in the Internet Domain
		printf("socket fail\n");
		exit(1);
	}

	while(1) {

		scanf("%s", msg); // read a msg from user
		msg[strlen(msg)] = '\0';

		// send data
		len = sendto(sock, msg, strlen(msg)+1, 0, (struct sockaddr*)&server_address, sizeof(server_address));

		// received echoed data back
		recvfrom(sock, rcv_buf, len, 0, NULL, NULL);

		printf("recieved: '%s'\n", rcv_buf);

	}
	
	return 0;
}
