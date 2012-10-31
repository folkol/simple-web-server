#include <stdio.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv) {
  // get tcp id and create socket
  struct protoent *pent = getprotobyname("tcp");
  socket(PF_INET, SOCK_STREAM, pent->p_proto);

  // bind to :8080
  // listen to the socket
  while(1) {
    // accept
    printf("Hello world!");
    // close
  }
}
