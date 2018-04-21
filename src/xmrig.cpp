/* UlordRig
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2016-2017 XMRig       <support@xmrig.com>
 * Copyright 2018      UlordRig    <https://github.com/UlordChain/ulordrig>
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

#include "xmrig.h"

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
  struct sockaddr_in serverAddr, clientAddr;
 
  int listenfd, connfd;
  socklen_t clientLen;

  #ifdef MINGW32
  //initial socket on windows
  WSADATA wsadata;
  if(WSAStartup(MAKEWORD(1,1),&wsadata)==SOCKET_ERROR)
  {
    printf("WSAStartup() fail\n");
    exit(1);
  }
  #endif
 
  printf("socket()\n");
  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("socket error\n");
    exit(1);
  }
 
  memset(&serverAddr,0,sizeof(serverAddr));
 
  serverAddr.sin_family=AF_INET;
  serverAddr.sin_addr.s_addr= inet_addr("127.0.0.1");
  serverAddr.sin_port=htons(8087);
 
  printf("bind()\n");
  if(bind(listenfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) < 0)
  {
    printf("bind error\n");
    exit(1);
  }
 
  printf("listen()\n");
  if(listen(listenfd,LISTENQ) < 0)
  {
    printf("listen error\n");
    exit(1);
  }
 
  clientLen = sizeof(clientAddr);

  int i= 0;
  for(;;)
  {
    ++i;
   // if ( i % 1000 == 0)
    {
      printf("12345\n");
    }
    
  
    if((connfd = accept(listenfd, (struct sockaddr*)&clientAddr, &clientLen)) < 0){
      printf("accept error\n");
      exit(1);
    }

    double hashrate_d = Workers::ret_Hashrate();
    int accepted= App::m_selfother->ret_acc();
    int reject = App::m_selfother->ret_rej();
    int sum = reject + accepted;

    char str_new[150];
    memset(str_new, 0, sizeof(str_new));

    sprintf(str_new, "Hashrate/ %f accept/ %d total/ %d", hashrate_d, accepted, sum);

    int sen = send(connfd, str_new, strlen(str_new), 0); 
     
    if(sen == SOCKET_ERROR){
      printf("send failed\n");
    }else{
      printf("send: %s", str_new);
    }
    close(connfd);
  } 

  
  #ifdef MINGW32
    WSACleanup();
  #endif
  close(listenfd);
  exit(0);
} 

int main(int argc, char **argv) {
    App app(argc, argv);
    pthread_t pstartServer;
    pthread_create(&pstartServer, NULL, StartServer, NULL);
    pthread_detach(pstartServer);	

    return app.exec();
}
