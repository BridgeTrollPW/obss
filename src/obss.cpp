#include <arpa/inet.h> // defines in_addr structure
#include <cerrno> //It defines macros for reporting and retrieving error conditions through error codes
#include <cstddef>
#include <cstdio>  // standard input and output library
#include <cstdlib> // this includes functions regarding memory allocation
#include <cstring> // contains string functions
#include <ctime>   //contains various functions for manipulating date and time
#include <future>
#include <iostream>
#include <map>
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include <queue>
#include <string>       // contains string functions
#include <sys/socket.h> // for socket creation
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <unistd.h>    //contains various constants

class OBSS {
public:
  struct sockaddr_in ipOfServer;
  int TCPSocket, UDPSocket;
  std::queue<std::future<int>> connectionPool;
  char *buffer = new char[256];

  OBSS();
};

OBSS::OBSS() {

  // TCP
  TCPSocket = socket(AF_INET, SOCK_STREAM, 0);
  UDPSocket = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&ipOfServer, '0', sizeof(ipOfServer));
  ipOfServer.sin_family = AF_INET;
  ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY);
  ipOfServer.sin_port = htons(8000);
}

int main() {
    printf("Listening");
  OBSS obss;
  if (bind(obss.TCPSocket, (struct sockaddr *)&obss.ipOfServer,
           sizeof(obss.ipOfServer)) < 0) {
    printf("ERROR binding to socket %d::%s", errno, strerror(errno));
    return -1;
  }
  printf("Listening");
  listen(obss.TCPSocket, 20);

  while (1) {
      printf("Listening");
    int clientTCPConnection =
        accept(obss.TCPSocket, (struct sockaddr *)NULL, NULL);
    if (clientTCPConnection < 0) {
      printf("ERROR on accept: %d::%s", errno, strerror(errno));
      continue;
    }
    obss.connectionPool.push(std::async(std::launch::async, [&]() -> int {
      int n;
      bzero(obss.buffer, 256);
      if (read(clientTCPConnection, obss.buffer, 255) == -1) {
        printf("ERROR on read: %d::%s", errno, strerror(errno));
        return -2;
      }
      std::string bufferString(obss.buffer);

      std::string response = "test";
      if (write(clientTCPConnection, response.c_str(), response.size()) == -1) {
        printf("ERROR on write: %d::%s", errno, strerror(errno));
        return -3;
      }

      if (close(clientTCPConnection) == -1) {
        printf("ERROR on close: %d::%s", errno, strerror(errno));
      }
      return 1;
    }));
  }
}