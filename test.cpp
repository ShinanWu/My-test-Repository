#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main()
{
    printf("test pritnf\n");
    string a("测试");
    cout << a << "\n";
    signal(SIGPIPE, SIG_IGN);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;    
memset( &server, 0, sizeof( struct sockaddr_in ) );
server.sin_family = AF_INET;
server.sin_port = htons(1234) ;
server.sin_addr.s_addr = inet_addr( "127.0.0.1" );

int res = -1;    
res = connect( sockfd, (struct sockaddr*)&server, sizeof( server ) );
if( -1 == res ){
    perror( "sock connect error" );
    exit( -1 );
}
 
 struct timeval tv = {5, 0};
int revlen;
// setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)); 
 setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
 //int nZero=0;
// setsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));
 char *buf = (char *)malloc(10*1024*1024);
printf("debug sizeof buf:%d\n", sizeof(buf));
 *buf ='a';
// int revlen = recv(sockfd, buf, 0, MSG_WAITALL);
// int revlen = 0;
// printf("retval: %d, errno:%s\n", revlen, strerror(errno));

  //revlen = recv(sockfd, buf, 0, MSG_WAITALL);
// int revlen = 0;
 //printf("retval: %d, errno:%s\n", revlen, strerror(errno));
//sleep(5);
//long long len;
//printf("sizeof long long: %d", sizeof(len));
 //revlen = recv(sockfd, buf, 0, MSG_WAITALL);
// int revlen = 0;
 //printf("retval: %d, errno:%s\n", revlen, strerror(errno));
//sleep(5);

 //revlen = recv(sockfd, buf, 0, MSG_WAITALL);
// int revlen = 0;
 //printf("retval: %d, errno:%s\n", revlen, strerror(errno));

//sleep(100);
  revlen = send(sockfd, buf, 1024*1024*10*10, 0);
 printf("2 retval: %d, errno:%s, errno:%d\n", revlen, strerror(errno), errno);
  revlen = send(sockfd, buf, 1024*1024*10*10, 0);
 printf("3 retval: %d, errno: %d , errno:%s\n", revlen, errno, strerror(errno));
 revlen = send(sockfd, buf, 1024*1024*10, 0);
 printf("4 retval: %d, errno:%s\n", revlen, strerror(errno));
 revlen = send(sockfd, buf, 1024*1024*10, 0);
 printf("5 retval: %d, errno:%s\n", revlen, strerror(errno));
 revlen = send(sockfd, buf, 1024*1024*10, 0);
 printf("6 retval: %d, errno:%s\n", revlen, strerror(errno));
 revlen = send(sockfd, buf, 1024*1024*10, 0);
 printf("7 retval: %d, errno:%s\n", revlen, strerror(errno));

}
