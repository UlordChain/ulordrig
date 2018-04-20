/* XMRig
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2016-2017 XMRig       <support@xmrig.com>
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "App.h"
#include <pthread.h>
#include <iostream>
#include "api/ApiState.h"
#include "workers/Hashrate.h"
#include "workers/Workers.h"
#include "api/NetworkState.h"
#include "net/Network.h"


#define MINGW32

 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#ifdef MINGW32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
 
#define LISTENQ 10
 
void *  StartServer(void*) 
{
  int serverfd,connectfd;
  struct sockaddr_in serveraddr;
 

  int iRet;
 
  #ifdef MINGW32
  //initial socket on windows
  WSADATA wsadata;
  if(WSAStartup(MAKEWORD(1,1),&wsadata)==SOCKET_ERROR)
  {
    printf("WSAStartup() fail\n");
    exit(0);
  }
  #endif
 
  printf("socket()\n");
  serverfd=socket(AF_INET,SOCK_STREAM,0);
  if(serverfd==-1)
  {
    printf("socket() fail\n");
    exit(0);
  }
 
  memset(&serveraddr,0,sizeof(serveraddr));
 
  serveraddr.sin_family=AF_INET;
  serveraddr.sin_addr.s_addr= inet_addr("127.0.0.1");
  serveraddr.sin_port=htons(8087);
 
  printf("bind()\n");
  iRet=bind(serverfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
  if(iRet==-1)
  {
    printf("bind() fail\n");
    exit(0);
  }
 
  printf("listen()\n");
  iRet=listen(serverfd,LISTENQ);
  if(iRet==-1)
  {
    printf("listen() fail\n");
    exit(0);
  }
 

  for(;;)
  {
   
    connectfd=accept(serverfd,(struct sockaddr*)NULL,NULL);
    
 
    #ifdef MINGW32
    
    //send(connectfd,buff,strlen(buff),0);
    
    char recvBuf[20]; 
    recv(connectfd, recvBuf, 20, 0);
    //printf(recvBuf);

    if (recvBuf[0] !='\0')
     {
       
   
            
              
           double  hashrate_d=Workers::ret_Hashrate();
            char str[7];
            _gcvt_s(str, sizeof(str), hashrate_d, 5);
            char str_h[30]="Hashrate/  ";
            strcat(str_h,str);
             int accepted= App::m_selfother->ret_acc();
             int reject = App::m_selfother->ret_rej();
             int sum =reject+accepted;

            char str_1[25],str_2[25];
              sprintf(str_1,"accept/ %d ", accepted );                  
              sprintf(str_2,"total/ %d",sum);
           char str_new[150];
           strcat(str_new,str_h);strcat(str_new,str_1);strcat(str_new,str_2);
           int sen=send(connectfd,str_new,strlen(str_new),0); 
           recvBuf[0]='\0';
           str_new[0]='\0';
           memset( str_new, '\0', sizeof(str_new) );

           //printf(str_new);
           if(sen == SOCKET_ERROR){
              printf("fault\n");
           }
            
           closesocket(connectfd);
           close(connectfd);
       }     
   
  }  
    #else
    //write(connectfd,buff,strlen(buff));
    
    #endif
 
 
  #ifdef MINGW32
  
  //closesocket(serverfd);
  WSACleanup();
  #endif
 
  exit(0);
} 




int main(int argc, char **argv) {
    App app(argc, argv);


    pthread_t pstartServer;
    pthread_create(&pstartServer,NULL,StartServer,NULL);
    pthread_detach(pstartServer);	
    return app.exec();
}
