#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

void server(int port) {
  sockaddr_in socket_in;
  sockaddr_in socket_from;

  int getted_socket;
  int slen = sizeof(socket_from), recv_len;
  std::vector<char> buffer(512, '0');

  if ((getted_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    return;
  }

  memset((char *)&socket_in, 0, sizeof(socket_in));

  socket_in.sin_family = AF_INET;
  socket_in.sin_port = htons(port);
  socket_in.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(getted_socket, (struct sockaddr *)&socket_in, sizeof(socket_in)) == -1) {
    close(getted_socket);
    return;
  }

  for (;;) {
    std::cout << "Waiting for data at port:" << port << std::endl;

    if ((recv_len = recvfrom(getted_socket, buffer.data(), buffer.size(), 0, (struct sockaddr *)&socket_from, (socklen_t *)&slen)) == -1) {
      close(getted_socket);
      return;
    }

    std::cout << "Received data from " << inet_ntoa(socket_from.sin_addr) << ':' << ntohs(socket_from.sin_port) << " at port " << port << std::endl;
    std::cout << "Data: " << std::string(buffer.data(), recv_len) << std::endl;
  }

  close(getted_socket);
}

int main(void) {

  std::vector<std::thread> threads;
  for (int i = 0; i < 3; i++) {
    int port = 1234 + i;
    threads.emplace_back(server, port);
  }

  for (auto it = threads.begin(); it != threads.end(); ++it) {
    it->join();
  }

  return 0;
}
