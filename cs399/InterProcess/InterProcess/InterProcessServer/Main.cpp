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

int main(void)
{
  printf("Starting\n");

  WSADATA wsaData;
  int result = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (result != 0)
  {
    throw socket_error("WSAStartup failed");
  }
  
  printf("Creating listening socket\n");
  SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (listenSocket == INVALID_SOCKET)
  {
    throw socket_error("Failed to create listen socket");
  }

  sockaddr_in name;
  name.sin_family = AF_INET;
  name.sin_port = htons(8000);
  name.sin_addr.s_addr = htonl(INADDR_ANY);
  
  printf("Binding listening socket\n");
  if (bind(listenSocket, (sockaddr*)&name, sizeof(name)) == SOCKET_ERROR)
  {
    throw socket_error("Failed to bind socket");
  }
  
  printf("Listening on listening socket\n");
  if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
  {
    throw socket_error("Failed to listen");
  }
  
  printf("Waiting for connection\n");
  SOCKET clientSocket = accept(listenSocket, NULL, NULL);
  if (clientSocket == INVALID_SOCKET)
  {
    throw socket_error("Failed to accept connection");
  }

  // Game loop!
  for (;;)
  {
    const int BufferSize = 4096;
    char buffer[BufferSize] = {0};
    
    unsigned long nonBlockingMode = 1;
    ioctlsocket(clientSocket, FIONBIO, &nonBlockingMode);

    int result = recv(clientSocket, buffer, BufferSize, 0);
    int lastError = WSAGetLastError();
    
    // We have to do this down here because 'ioctlsocket' can change 'WSAGetLastError'
    nonBlockingMode = 0;
    ioctlsocket(clientSocket, FIONBIO, &nonBlockingMode);

    // If the editor disconnected...
    if (result == 0)
    {
      printf("Disconnected");
      break;
    }

    if (result == SOCKET_ERROR)
    {
      if (lastError != WSAEWOULDBLOCK)
        throw socket_error("An error occurred while receiving");
    }
    else
    {
      printf("Received: %s\n", buffer);
    }

    send(clientSocket, "World", 5, 0);

    // Simulate other things (graphics, physics, etc)
    Sleep(16);
  }
  
  printf("Exiting\n");
  WSACleanup();

  system("pause");
  return 0;
}
