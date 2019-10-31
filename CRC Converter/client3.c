#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
int main()
{
	int sock;
	// client socket discriptor
	int a,b,c,i;
	unsigned int len;
	char ch[3]="no";
	char ch1[3];
	struct sockaddr_in client;
	if((sock=socket(AF_INET,SOCK_STREAM,0))==-1)
	// client socket is created.
	{
		perror("socket: ");
		exit(-1);
	}
	client.sin_family=AF_INET;
	client.sin_port=htons(10000);
	// initializing socket parameters
	client.sin_addr.s_addr=INADDR_ANY;
	bzero(&client.sin_zero,0);
	// appending 8 byte zeroes to 'struct sockaddr_in', to make it equal in size with 'struct sockaddr'.
	len=sizeof(struct sockaddr_in);
	if((connect(sock,(struct sockaddr *)&client,len))==-1)
	//connecting to client
	{
		perror("connect: ");
		exit(-1);
	}
	while(1)
	{
		char pass[100],div[100],str[100],pad[100];
		printf("Enter the Passowrd:\n");
		scanf("%s",pass);
		send(sock,pass,sizeof(pass),0);
		recv(sock,&a,sizeof(a),0);
		if(a==1)
		{
			printf("Hi Server, I'm Client!!\n");
			printf("Enter the Common Divisor\n");
            scanf("%s",div);
			send(sock,div,sizeof(div),0);
			printf("Enter the Bit String\n");
			scanf("%s",str);
			strcpy(pad,str);
            for(i=0;i<strlen(div)-1;i++)
			strcat(pad,"0");
			send(sock,str,sizeof(str),0);
			send(sock,pad,sizeof(pad),0);
		}
		else
			printf("Incorrect Password, Sorry :(\n");
		printf("\nTo exit...press 'no'\n");
		scanf("%s",ch1);
		if((i=strcmp(ch,ch1))==0)
		{
			close(sock);
			exit(0);
		}
	}
}
