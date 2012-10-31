#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
  int listen_socket;
  int client_socket;
  struct sockaddr_in listen_sock_addr;
  struct sockaddr current_client;
  int current_client_len;

  // set state
  errno = 0;
  listen_sock_addr.sin_family = AF_INET;
  listen_sock_addr.sin_addr.s_addr = INADDR_ANY;
  listen_sock_addr.sin_port = htons(8080);

  // create socket
  if((listen_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    perror("socket");
    return -1;
  }

  //make it reusable
  int sockopt_on = 1;
  if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt_on, sizeof(int)) == -1) {
    perror("setsockopt");
    return 1;
  }

  // bind to :8080
  if(bind(listen_socket, (struct sockaddr *) &listen_sock_addr, sizeof(listen_sock_addr)) == -1) {
    perror("bind");
    return -1;
  }

  // listen to the socket
  if(listen(listen_socket, 100) == -1) {
    perror("listen");
    return -1;
  }

  // enter listen loop
  while(1) {
    // accept
    current_client_len = sizeof(current_client);
    if((client_socket = accept(listen_socket,
                               (struct sockaddr*) &current_client,
                               (socklen_t*)&current_client_len)) == -1) {
      perror("accept");
      break;
    }

    fprintf(stderr, "Hello world!\n");
    
    // close
    if(close(client_socket) == -1) {
      perror("close");
      break;
    }
  }


  /*  struct sockaddr_in serv_addr; 


struct sockaddr {
    unsigned short sa_family;   // address family, AF_xxx
    char           sa_data[14]; // 14 bytes of protocol address
};



  char sendBuff[1025];
  time_t ticks; 

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));
  memset(sendBuff, '0', sizeof(sendBuff)); 

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(5000); 

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

  listen(listenfd, 10); 

  while(1)
    {
      connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

      ticks = time(NULL);
      snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
      write(connfd, sendBuff, strlen(sendBuff)); 

      close(connfd);
      sleep(1);
    }
  */
}
