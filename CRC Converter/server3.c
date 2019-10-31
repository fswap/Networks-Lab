#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
int cmp(char word[],char capital[])
{
	int n1=sizeof(word)/sizeof(word[0]);
	int n2=sizeof(capital)/sizeof(capital[0]);
	if(n1!=n2)
	return 1;
	int i;	
	for(i=0;i<n1;i++)
	{
		if(word[i]!=capital[i])
			return 1;
		if(word[i]=='\0' && capital[i]=='\0')
			return 0;	
	}
	return 0;
}
int main()
{
	int ssock,csock;
	// creating server and client socket discriptor
	int a=0,i,j;
	unsigned int len;
	struct sockaddr_in server,client;
	// creating server & client socket object
	if((ssock=socket(AF_INET,SOCK_STREAM,0))==-1)
	// creating socket
	{
		perror("socket: ");
		exit(-1);
	}
	server.sin_family=AF_INET;
	server.sin_port=htons(10000);
	// initializing server socket parameters.
	server.sin_addr.s_addr=INADDR_ANY;
	bzero(&server.sin_zero,0);
	// appending 8 byte zeroes to 'struct sockaddr_in', to make it equal in size with 'struct sockaddr'.
	len=sizeof(struct sockaddr_in);
	if((bind(ssock,(struct sockaddr *)&server,len))==-1)
	// binding port & IP
	{
		perror("bind error: ");
		exit(-1);
	}
	if((listen(ssock,5))==-1)
	// listening for client
	{
		perror("listen error: ");
		exit(-1);
	}
	if((csock=accept(ssock,(struct sockaddr *)&client,&len))==-1)
	// accepting connection
	{
		perror("accept error: ");
		exit(-1);
	}
	while(1)
	{
		char pass[100],div[100],str[100],ans[100]="",pad[100];
		recv(csock,pass,sizeof(pass),0);
		if(strcmp(pass,"ismcse")==0)
			a=1;
		send(csock,&a,sizeof(a),0);
		if(a==1)
		{
			recv(csock,div,sizeof(div),0);
			printf("Hello Client, The Common Divisor is: %s\n", div);
			recv(csock,str,sizeof(str),0);
			recv(csock,pad,sizeof(pad),0);
			printf("Received Padded Bit String is: %s\n",pad);
			for(i=0;i<strlen(pad);)
			{
				strcat(ans,"1");
				for(j=0;j<strlen(div);j++)
					pad[i+j]=pad[i+j]^div[j];
				for(j=i+1;pad[j]!=1;j++)
					strcat(ans,"0");
				i=j;
				if(strlen(pad)-i<strlen(div))
					break;
			}
			printf("After one phase: %s\n",ans);
		}
		// sending data to client.
		printf("\ntask completed\n");
	}
	close(ssock);
}
