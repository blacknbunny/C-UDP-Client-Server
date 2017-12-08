#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char **argv){

  if(argc != 3){
    puts("Usage : ./tcpclient hostname port");
    exit(1);
  }
  /* Hostname to ip Starts here */
  char ip[100];
  struct hostent *he;
  struct in_addr **addr_list;

  if ( (he = gethostbyname( argv[1] ) ) == NULL)
  {
      herror("gethostbyname ");
      return 1;
  }

  addr_list = (struct in_addr **) he->h_addr_list;

  for(int i = 0; addr_list[i] != NULL; i++)
  {
      strcpy(ip , inet_ntoa(*addr_list[i]) );
  }
  /* Ends Here */

  int sockfd, received_bytes, sended_bytes;
  char buffer[1024], buffer_to_send[1024];

  struct sockaddr_in local_addr;
  struct sockaddr_in remote_addr;

  int slen = sizeof(remote_addr);

  while(1){
    if((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
      perror("Socket ");
      exit(1);
    }

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(atoi(argv[2]));
    remote_addr.sin_addr.s_addr = inet_addr(ip);
    memset(remote_addr.sin_zero, '\0', sizeof(remote_addr.sin_zero));

    int remote_connect = connect(sockfd, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr));
    if(remote_connect == -1){
      perror("Connect ");
      exit(1);
    }

    printf("Message : ");
    fgets(buffer_to_send, sizeof(buffer)-1, stdin);

    if((sended_bytes = sendto(sockfd, buffer_to_send, strlen(buffer_to_send), 0, (struct sockaddr*) &remote_addr, slen)) == -1){
      perror("Send ");
      exit(1);
    }

    buffer[sended_bytes] = '\0';

    if((received_bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&remote_addr, &slen)) == -1){
      perror("Recv ");
      exit(1);
    }

    buffer[received_bytes] = '\0';

    printf("Received : %s\n", buffer);
  }

  return 0;
}
