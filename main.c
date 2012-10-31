#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// glooooooobals
int listen_socket;
int client_socket;
struct sockaddr_in listen_sock_addr;
struct sockaddr current_client;
int current_client_len;

void init_socket() {
  // set state
  errno = 0;
  listen_sock_addr.sin_family = AF_INET;
  listen_sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  listen_sock_addr.sin_port = htons(8080);

  // create socket
  if((listen_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
    perror("socket");
    exit(-1);
  }

  //make it reusable
  int sockopt_on = 1;
  if (setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &sockopt_on, sizeof(int)) == -1) {
    perror("setsockopt");
    exit(-1);
  }

  // bind to :8080
  if(bind(listen_socket, (struct sockaddr *) &listen_sock_addr, sizeof(listen_sock_addr)) == -1) {
    perror("bind");
    exit(-1);
  }

  // listen to the socket
  if(listen(listen_socket, 100) == -1) {
    perror("listen");
    exit(-1);
  }
}

void handle_request() {
  // enter listen loop
  char send_buffer[1024];
  char* status_line = "HTTP/1.1 200 OK\x0D\x0A";
  char* header1 = "Content-Type: text/plain";
  char* header2 = "Cache-Control: max-age=300";
  char* body = "Hello World!";
  int message_length;

  //  "Method SP Request-URI SP HTTP-Version CRLF";
  // compose response
  message_length = 0;
  strcpy(send_buffer, status_line);
  message_length += strlen(status_line);
  
  strcat(send_buffer, header1);
  message_length += strlen(header1);
  strcat(send_buffer, "\x0D\x0A");
  message_length += 2;
  
  strcat(send_buffer, header2);
  message_length += strlen(header2);
  strcat(send_buffer, "\x0D\x0A");
  message_length += 2;
  
  strcat(send_buffer, "\x0D\x0A");
  message_length += 2;
  strcat(send_buffer, body);
  message_length += strlen(body);
  
  if(send(client_socket, send_buffer, message_length, 0) == -1) {
    perror("send");
    exit(-1);
  }
  
  fprintf(stderr, "Served!\n");
  
  // close
  if(close(client_socket) == -1) {
    perror("close");
    exit(-1);
  }
}

int main(int argc, char **argv) {
  init_socket();

  while(1) {
    current_client_len = sizeof(current_client);
    if((client_socket = accept(listen_socket,
                               (struct sockaddr*) &current_client,
                               (socklen_t*)&current_client_len)) == -1) {
      perror("accept");
      break;
    }

    handle_request();
  }

  return 0;
}
