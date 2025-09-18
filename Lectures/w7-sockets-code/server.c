#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define PORT      9876

char buf[2048];

int main(int argc, char *argv[]) {

	int sock;
	int len;

	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	int client_address_len = sizeof(client_address);

	server_address.sin_family = AF_INET; // IPv4 Address
	server_address.sin_port = htons(PORT);  // Set port number
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // Accept messages from anyone


	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) { // Open a UDP Socket in the Internet domain
		printf("socket fail\n");
		exit(1);
	}

	if ((bind(sock, (struct sockaddr *)&server_address, sizeof(server_address))) < 0) { // Bind the socket to the address
		printf("bind fail\n");
		exit(1);
	}

	printf("Server online at port: %d\n", PORT);

	while (1) { // run forever

		// block until we receive a message
		len = recvfrom(sock, buf, sizeof(buf), 0,	(struct sockaddr *)&client_address, &client_address_len);

		buf[len] = '\0';
		printf("msg received: '%s' from client %s\n", buf, inet_ntoa(client_address.sin_addr));

		// echo the message back
		sendto(sock, buf, len, 0, (struct sockaddr *)&client_address, sizeof(client_address));
	}

	return 0;
}
