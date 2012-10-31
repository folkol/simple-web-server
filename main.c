#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
  // create socket
  int my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  // bind to :8080
  struct sockaddr my_sock_addr;
  //  my_sock_addr.
    //  bind(my_socket, (struct sockaddr *) &my_sock_addr, sizeof(struct sockaddr), 2);
  // listen to the socket
  while(1) {
    // accept
    printf("Hello world!");
    // close
  }
}
