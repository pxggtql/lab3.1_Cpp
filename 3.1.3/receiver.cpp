#include <stdio.h>   
#include <sys/types.h>   
#include <sys/socket.h>   
#include <netinet/in.h>   
#include <unistd.h>   
#include <errno.h>   
#include <string.h>   
#include <stdlib.h> 
#include <iostream>  
#include <string>
#include "util.h"
using namespace std;
#define SERV_PORT   8000   
int main()  
{  
  /* sock_fd --- socket文件描述符 创建udp套接字*/  
  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock_fd < 0)  
  {  
    perror("socket");  
    exit(1);  
  }  
  
  /* 将套接字和IP、端口绑定 */  
  struct sockaddr_in addr_serv;  
  int len;  
  memset(&addr_serv, 0, sizeof(struct sockaddr_in));  //每个字节都用0填充
  addr_serv.sin_family = AF_INET;  //使用IPV4地址
  addr_serv.sin_port = htons(SERV_PORT); //端口
  /* INADDR_ANY表示不管是哪个网卡接收到数据，只要目的端口是SERV_PORT，就会被该应用程序接收到 */  
  addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);  //自动获取IP地址
  len = sizeof(addr_serv);  
    
  /* 绑定socket */  
  if(bind(sock_fd, (struct sockaddr *)&addr_serv, sizeof(addr_serv)) < 0)  
  {  
    perror("bind error:");  
    exit(1);  
  }  
  int recv_num;  
  int send_num;  
  char send_buf[20]="1";  
  unsigned char recv_buf[20];  
  struct sockaddr_in addr_client;  
  int index=0;
  FILE *fp = NULL;
  fp = fopen("receiveFile", "w+");
  while(1)  
  {  
    memset(recv_buf,0,sizeof(recv_buf));
    printf("[Message]Receiver wait frame\n");  
    recv_num = recvfrom(sock_fd, recv_buf, 6, 0, (struct sockaddr *)&addr_client, (socklen_t *)&len);
    for(int z=0;z<6;z++)
    printf("%x ",recv_buf[z]);
    uint16_t crcResult=crc16(recv_buf,6);
    printf("%d",crcResult);
    if(crcResult==0)
    {
      index++;
      printf("[Success]Frame %d received!\n",index);
      for(int i=0;i<4;i++)
      {
      fputc(recv_buf[i],fp );
      }
      send_num = sendto(sock_fd, send_buf, 2, 0, (struct sockaddr *)&addr_client, len); 
      if(send_num < 0)  
    {  
      perror("sendto error:");  
      exit(1);  
    }   
    }
    else
    {
      cout<<"[Error]Received frame is wrong!\n";
      char wrong_buf[20]="-1";
      send_num = sendto(sock_fd, wrong_buf, 2, 0, (struct sockaddr *)&addr_client, len); 
      if(send_num < 0)  
    {  
      perror("sendto error:");  
      exit(1);  
    }   

    }
    
  }  
    
  close(sock_fd);  
    
  return 0;  
}