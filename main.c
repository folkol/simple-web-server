#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>

// glooooooobals
int listen_socket;
int client_socket;
struct sockaddr_in listen_sock_addr;
struct sockaddr current_client;
char send_buffer[1024*1024];

char recv_buffer[1024*1024];
char request_method[10];
char request_uri[1024];

char filename[1024];
char filebuffer[1024*1024];

char* www_root = "www-root";
char* status_line_ok = "HTTP/1.1 200 OK\x0D\x0A";
char* status_line_not_found = "HTTP/1.1 404 Not Found\x0D\x0A\x0D\x0A";

// Functions
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
  // read request
  int message_len;
  if((message_len = recv(client_socket, &recv_buffer, sizeof(recv_buffer), 0)) == -1) {
    perror("recv");
    exit(-1);
  }

  // parse status line "Method SP Request-URI SP HTTP-Version CRLF";
  sscanf(recv_buffer, "%s %s", request_method, request_uri);

  // No uri path given, assume index.html
  if(strlen(request_uri)==1) {
    strcpy(request_uri, "/index.html");
  }

  // open requested file
  sprintf(filename, "%s%s", www_root, request_uri);

  FILE *filepointer;
  int character;
  filepointer=fopen(filename, "r"); /* filepointer points to data.txt */
  if (filepointer==NULL) { /* error opening file returns NULL */
    fprintf(stderr, "Requsted file %s not found, sent 404 as response to (METHOD:%s URI:%s)\n",
            filename, request_method, request_uri);
    // write status
    if(send(client_socket, status_line_not_found, strlen(status_line_not_found), 0) == -1) {
      perror("send");
      exit(-1);
    }
    // close
    if(close(client_socket) == -1) {
      perror("close");
      exit(-1);
    }
    return;
  }

  // write status
  if(send(client_socket, status_line_ok, strlen(status_line_ok), 0) == -1) {
    perror("send");
    exit(-1);
  }

  // write header/body reparator  
  if(send(client_socket, "\x0D\x0A", 2, 0) == -1) {
    perror("send");
    exit(-1);
  }

 /* while character is not end of file */
  while ((character=fgetc(filepointer)) != EOF) {
    send_buffer[0] = character;
    // write body
    if(send(client_socket, send_buffer, 1, 0) == -1) {
      perror("send");
      exit(-1);
    }
  }
  fclose(filepointer); /* close the file */
  
  // close
  if(close(client_socket) == -1) {
    perror("close");
    exit(-1);
  }
}

int main(int argc, char **argv) {
  init_socket();

  socklen_t current_client_len;
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
