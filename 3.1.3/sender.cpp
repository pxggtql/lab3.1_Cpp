#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "util.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
#define DEST_PORT	8000
#define DSET_IP_ADDRESS "localhost"
int sum;
int i=0;
int main()
{
	i++;
  sum=0;
	/* socket文件描述符 */
	int sock_fd;
	/* 建立udp socket */
	sock_fd = socket( AF_INET, SOCK_DGRAM, 0 );
	if ( sock_fd < 0 )
	{
		perror( "socket" );
		exit( 1 );
	}
	/* 设置address */
	struct sockaddr_in	addr_serv;
	int			len;
	memset( &addr_serv, 0, sizeof(addr_serv) );
	addr_serv.sin_family		= AF_INET;
	addr_serv.sin_addr.s_addr	= inet_addr( DSET_IP_ADDRESS );
	addr_serv.sin_port		= htons( DEST_PORT );
	 int on=1;
  setsockopt(sock_fd,SOL_SOCKET,SO_REUSEADDR | SO_BROADCAST,&on,sizeof(on));
  struct timeval timeOut;
	timeOut.tv_sec	= 2;                     /* 设置2s超时 */
	timeOut.tv_usec = 0;
	len				= sizeof(addr_serv);
	int		send_num;
	int		recv_num;
	unsigned char	send_buf[20];
	char		recv_buf[20];
	int		sum1;
  cout<<"[Message]All frames are ready to sent!Frame number is "<<sum1<<endl;;
	FILE *fp;
	fp=fopen("1.txt","r");
	fseek(fp,0,SEEK_SET);
	while(!feof(fp))
	{
    cout<<"[Message]frame number is "<<i<<endl;
	i++;
    int flag=0;
		memset( send_buf, 0, sizeof(send_buf) );
		unsigned char sendChar[4];
		cout<<"read"<<endl;
		fread(sendChar,sizeof(unsigned char),4,fp);
		uint16_t crcResult	= crc16( sendChar, 4 );
		uint8_t crcData[2];
		memset(crcData,0,sizeof(crcData));
		cout<<crcResult<<endl;
		crcData[1]=crcResult&0xFF;
		crcResult>>=8;
		crcData[0]=crcResult&0xFF;
		send_buf[0]=sendChar[0];
		send_buf[1]=sendChar[1];
		send_buf[2]=sendChar[2];
		send_buf[3]=sendChar[3];
		send_buf[4]	= crcData[0]; 
		send_buf[5]	= crcData[1];
		for(int x=0;x<6;x++)
		{
			printf("%x ",send_buf[x]);
		}
		printf("\n");
    if(rand()%10==0)
    {flag=1;
    unsigned char wrong_buf[20];
    memcpy(wrong_buf,send_buf,sizeof(send_buf));
    wrong_buf[3]=rand()%128;
    cout<<"[Message]Send a wrong frame！"<<endl;
    send_num	= sendto( sock_fd, wrong_buf, 6, 0, (struct sockaddr *)&addr_serv, len );
    sum++;
    }
    else if(rand()%10==11)
    {flag=1;
    cout<<"[Message]Send frame lost!"<<endl;}
    if(flag==0)
	{send_num	= sendto( sock_fd, send_buf, 6, 0, (struct sockaddr *)&addr_serv, len );
	if ( send_num < 0 )
		{
			perror( "sendto error:" );
			exit( 1 );
		}
    sum++;
    }
		
    printf( "[Message]Server wait response...\n" );
		if ( setsockopt( sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeOut, sizeof(timeOut) ) < 0 )
		{
			printf( "time out setting failed\n" );
		}
		int receivePacketLen = recvfrom( sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_serv, (socklen_t *)len);
		if ( receivePacketLen == -1 && errno == EAGAIN )       
		{
		 printf( "[Error]timeout, frame is lost! Resending...\n" );
		 send_num	= sendto( sock_fd, send_buf, 6, 0, (struct sockaddr *) &addr_serv, len );
     sum++;
     cout<<"[Success]Receiver receive a frame correctly!\n";
     int receivePacketLen = recvfrom( sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&addr_serv, (socklen_t *)len);
     continue;
		}
    if(recv_buf[0]=='1')
      {
        cout<<"[Success]Receiver receive a frame correctly!\n";
      }
    if(recv_buf[0]=='-')
    {
      cout<<"[Error]Receiver receive a wrong frame!Resending\n";
      send_num	= sendto( sock_fd, send_buf, 6, 0, (struct sockaddr *) &addr_serv, len );
      sum++;
      cout<<"[Success]Receiver receive a frame correctly!\n";
    }
	sleep(2);
	}
  cout<<"[Message]send all frames successfully!"<<endl;
	close( sock_fd );

	return(0);
}