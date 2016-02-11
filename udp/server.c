#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 8000
#define MAXLINE 1500

int main(void)
{
	int sockfd, i;
	ssize_t len;
	struct sockaddr_in serveraddr, clientaddr;
	char buf[MAXLINE];
	char ipstr[INET_ADDRSTRLEN]; // 16 Bytes
	socklen_t clientlen;

	//构造用于UDP通信的套接字
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET; //设置IPv4
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); //本地任意IP
	serveraddr.sin_port = htons(SERVER_PORT);

	bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

	while(1)
	{
		clientlen = sizeof(clientaddr);
		len = recvfrom(sockfd, 
				buf, 
				sizeof(buf), 
				0, 
				(struct sockaddr *)&clientaddr, 
				&clientlen
			);

		printf("client IP %s\tPORT%d\n", 
				inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, ipstr, sizeof(ipstr)),
				ntohs(clientaddr.sin_port)
		      );	

		i = 0;
		while(i < len)
		{
			buf[i] = toupper(buf[i]);
			i++;
		}

		sendto(sockfd, 
				buf, 
				len, 
				0, 
				(struct sockaddr *)&clientaddr, 
				sizeof(clientaddr)
		      );
	}

	close(sockfd);
	return 0;	
}
