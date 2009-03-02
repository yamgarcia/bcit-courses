#include "client.h"
#include "tools.h"

void start_client(char * server, char * port) {
	struct addrinfo hints, *serverinfo, *p;
	char ass[INET6_ADDRSTRLEN];
	char sendbuf[MAXLEN], recvbuf[MAXLEN];
	int sockfd;
	int ret;
	
	memset(&hints,0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	if((ret = getaddrinfo(server, port, &hints, &serverinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
		exit(1);
	}

	for (p = serverinfo; p !=NULL; p = p->ai_next ) {

		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			fprintf(stdout, "Socket create fail.\n");
			continue;
		}

		if ((connect(sockfd, p->ai_addr, p->ai_addrlen))==-1) {
			fprintf(stdout, "connection fail.\n");
			continue;
		}
		break;
	}

	if (p == NULL) {
		perror("failed to connect.\n");
		exit(2);
	}
	
	inet_ntop(p->ai_family,get_in_addr((struct sockaddr*)p->ai_addr),ass,sizeof(ass));

	freeaddrinfo(serverinfo);

	while (fgets(sendbuf, MAXLEN, stdin)) {
		if ((send(sockfd, sendbuf, strlen(sendbuf), 0))==-1) {
			fprintf(stdout, "send failed\n");
		}
		
		if ((recv(sockfd, recvbuf, MAXLEN, 0))==-1) {
			fprintf(stdout, "recv failed\n");
		}
		
		printf("%s", recvbuf);
	}
}
