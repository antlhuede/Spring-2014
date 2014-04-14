#include "Precompiled.h"
#include <Winsock2.h>
#include <stdio.h>
#include <exception>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

struct socket_error : exception
{
  socket_error(const string& str) : message(str) {}
  string message;
  const char* what() const { return message.c_str(); }
};

int fuzzle(void)
{
  WSADATA wsaData;
  int result = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (result != 0)
  {
    throw socket_error("WSAStartup failed");
  }

  SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (listenSocket == INVALID_SOCKET)
  {
    throw socket_error("Failed to create listen socket");
  }

  sockaddr_in name;
  name.sin_family = AF_INET;
  name.sin_port = htons(8000);
  name.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(listenSocket, (sockaddr*)&name, sizeof(name)) == SOCKET_ERROR)
  {
    throw socket_error("Failed to bind socket");
  }

  if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
  {
    throw socket_error("Failed to listen");
  }

  SOCKET clientSocket = accept(listenSocket, NULL, NULL);
  if (clientSocket == INVALID_SOCKET)
  {
    throw socket_error("Failed to accept connection");
  }

  WSACleanup();
  return 0;
}
