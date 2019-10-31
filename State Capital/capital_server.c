#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
int cmp(char word[],char mean[])
{
	int n1=sizeof(word)/sizeof(word[0]);
	int n2=sizeof(mean)/sizeof(mean[0]);
	if(n1!=n2)
	return 1;
	int i;	
	for(i=0;i<n1;i++)
	{
		if(word[i]!=mean[i])
			return 1;
		if(word[i]=='\0' && mean[i]=='\0')
			return 0;	
	}
	return 0;
}
int main()
{
	int ssock,csock;
	// creating server and client socket discriptor
	int a,b,c;
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
		char word[100],mean[100];
		recv(csock,word,sizeof(word),0);
		FILE *fptr;
		fptr=fopen("capital.txt","r");
		if(!fptr)
		{
			printf("FILE not open");
		}
		else
		{
			char ch[100];
			int flag=0;
			while(fgets(ch,100,fptr))
			{
				printf("%s\n",ch);
				if(cmp(word,ch)==0)
				{
					flag=1;
					fgets(ch,100,fptr);
					break;
				}
			}
			fclose(fptr);
			a=1;
			if(flag==1)
			{
				send(csock,&a,sizeof(a),0);
				send(csock,ch,sizeof(ch),0);
			}
			else
			{
				a=-a;
				send(csock,&a,sizeof(a),0);
				recv(csock,mean,sizeof(mean),0);
				fptr=fopen("capital.txt","a");
				if(!fptr)
				{
					printf("FILE NOT OPEN\n");
				}
				else
				{
					fputs(word,fptr);
					fputs("\n",fptr);
					fputs(mean,fptr);
					fputs("\n",fptr);
					fclose(fptr);
				}
			}					
		}
		// sending data to client.
		printf("\ntask completed\n");
	}
	close(ssock);
}
