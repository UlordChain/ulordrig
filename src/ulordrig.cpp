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

#ifdef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <iostream>
#include <winsock2.h>

#include "api/ApiState.h"
#include "workers/Hashrate.h"
#include "workers/Workers.h"
#include "api/NetworkState.h"
#include "net/Network.h"

#define LISTENQ 10
 
void *StartServer(void*) 
{
  struct sockaddr_in serverAddr, clientAddr;
 
  SOCKET  listenfd, connfd;
  socklen_t clientLen;
  char sendBuf[150];
  memset(sendBuf, 0, sizeof(sendBuf));
 
  //initial socket on windows
  WSADATA wsadata;
  if(WSAStartup(MAKEWORD(1,1),&wsadata) != 0)
  {
    printf("WSAStartup() fail\n");
    //return;
    exit(1);
  } 

  if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
  {
    printf("socket error\n");
    //return;
    exit(1);
  }
 
  memset(&serverAddr,0,sizeof(serverAddr));
 
  serverAddr.sin_family=AF_INET;
  serverAddr.sin_addr.s_addr= inet_addr("127.0.0.1");
  serverAddr.sin_port=htons(8087);
 
  if(bind(listenfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == SOCKET_ERROR)
  {
    printf("bind error\n");
    //return;
    exit(1);
  }
 
  if(listen(listenfd,LISTENQ) == SOCKET_ERROR)
  {
    printf("listen error\n");
    //return;
    exit(1);
  }
 
  clientLen = sizeof(clientAddr);

  while(true)
  {
  
    if((connfd = accept(listenfd, (struct sockaddr*)&clientAddr, &clientLen)) == INVALID_SOCKET)
    {
      printf("accept error\n");
      continue;
    }

    double hashrate_d = Workers::ret_Hashrate();
    int accepted = App::m_selfother->ret_acc();
    int reject = App::m_selfother->ret_rej();
    int sum = reject + accepted;

    memset(sendBuf, 0, sizeof(sendBuf));

    sprintf(sendBuf, "Hashrate/ %f accept/ %d total/ %d", hashrate_d, accepted, sum);
    send(connfd, sendBuf, strlen(sendBuf), 0);

    closesocket(connfd);
  } 

  closesocket(listenfd);
  WSACleanup();
  exit(0);
}

#endif

int main(int argc, char **argv) 
{
    App app(argc, argv);

    #ifdef WIN32
    pthread_t pstartServer;
    pthread_create(&pstartServer, NULL, StartServer, NULL);
    pthread_detach(pstartServer);	
    #endif

    return app.exec();
}
