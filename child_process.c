#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 80

int main(int argc, char const *argv[])
{
printf("Start of child process \n");
int valread;
char buffer[1024] = {0};
int new_socket = atoi(argv[1]);
valread = read( new_socket , buffer, 1024); 
printf("%s\n",buffer ); 
send(new_socket , argv[2] , strlen(argv[2]) , 0 ); 
printf("Hello message sent\n");
printf("End of child process \n");
return 0;
}
