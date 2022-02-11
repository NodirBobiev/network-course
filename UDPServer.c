#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include<netinet/in.h>
#define PORT 5000
#define MAXLINE 1000

int main()
{
  char buffer[100];
  int listenfd, len;
  struct sockaddr_in servaddr, cliaddr;
  struct sockaddr_in user_a, user_b;
  int flag1 = 0, flag2 = 0;
  bzero(&servaddr, sizeof(servaddr));


  listenfd = socket(AF_INET, SOCK_DGRAM, 0);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(PORT);
  servaddr.sin_family = AF_INET;


  bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));

  while( 1 )
  {
    len = sizeof(cliaddr);
    int n = recvfrom(listenfd, buffer, sizeof(buffer),
        0, (struct sockaddr*)&cliaddr,&len); 
        if( flag1 == 0 )
            user_a = cliaddr, flag1 = 1;
        if( flag1 == 1 && flag2 == 0 && cliaddr.sin_addr.s_addr != user_a.sin_addr.s_addr )
            user_b = cliaddr, flag2 = 1;
    buffer[n] = '\0';


    if( flag1 == 1 && flag2 == 1 && user_a.sin_addr.s_addr == cliaddr.sin_addr.s_addr ){
            sendto(listenfd, buffer, MAXLINE, 0,
                (struct sockaddr*)&user_b, sizeof(user_b));
            printf("user_a: %s\n", buffer);
        }
        else if( flag1 == 1 && flag2 == 0 ){
            printf("user_a: %s\n", buffer);
        }
        if( flag1 == 1 && flag2 == 1 && user_b.sin_addr.s_addr == cliaddr.sin_addr.s_addr ){
            sendto(listenfd, buffer, MAXLINE, 0,
                (struct sockaddr*)&user_a, sizeof(user_a));
            printf("user_b: %s\n", buffer);
        }
  }
}