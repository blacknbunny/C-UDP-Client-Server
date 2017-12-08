#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

struct configuration {
  int myport;
  int backlog;
};


int main(int argc, char **argv){
  if(argc != 2){
    puts("Usage : ./tcpserver port");
    exit(1);
  }
  int sockfd, received_bytes, sended_bytes;
  char buffer[1024], buffer_to_send[1024];

  struct configuration conf={atoi(argv[1]), 20};
  struct configuration *confptr;
  confptr = &conf;

  struct sockaddr_in local_addr;
  struct sockaddr_in remote_addr;

  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if(sockfd == -1){
    perror("Socket error ");
    return 1;
  }

  char tr = '1';

  if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&tr,sizeof(int)) == -1) {
    perror("Setsockopt ");
    exit(1);
  }


  local_addr.sin_family = AF_INET;
  local_addr.sin_port = htons(confptr->myport);
  local_addr.sin_addr.s_addr = INADDR_ANY;
  memset(local_addr.sin_zero, '\0', sizeof(local_addr.sin_zero));

  int local_bind = bind(sockfd, (struct sockaddr *)&local_addr, sizeof(struct sockaddr));
  if(local_bind == -1){
    perror("Bind error ");
    return 1;
  }

  int slen = sizeof(remote_addr);

  while(1){
    if((received_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&remote_addr, &slen)) == -1){
      perror("Recv ");
      exit(1);
    }

    buffer[received_bytes] = '\0';

    printf("Received : %s\n", buffer);

    printf("Message : ");
    fgets(buffer_to_send, sizeof(buffer), stdin);


    if((sended_bytes = sendto(sockfd, buffer_to_send, strlen(buffer_to_send), 0, (struct sockaddr*) &remote_addr, slen)) == -1){
      perror("Send error ");
      return 1;
    }

    buffer_to_send[sended_bytes] = '\0';

  }

  return 0;

}
