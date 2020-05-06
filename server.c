// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <unistd.h> 
#define PORT 80 

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, valread; 
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address); 
    uid_t new_pid;
    char *hello = "Hello from server"; 
    int status; // Used to check setuid result
    
    char socket_str[10];

    // Storing the UID and Effective UID.   
    int real = getuid();
    int euid = geteuid();

    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 80 
    //if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 80 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(server_fd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    } 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,  
                   (socklen_t*)&addrlen))<0) 
    { 
	    perror("accept"); 
	    exit(EXIT_FAILURE); 
    } 
    new_pid = fork();
    if (new_pid < 0){
	    perror("ERROR on fork");
	    exit(0);
    }    
    else if (new_pid == 0){
	    // Lowering the privilege by using setuid(newid)
	    status = setuid(real);
	    // Check for the status of setuid, if it is negative, the setuid command failed 
	    if (status < 0) {
		    perror("ERROR on setuid 1");
	    }
	    else {
		    sprintf(socket_str, "%d", new_socket);
		    char *args[] = {"./child_process", socket_str, hello, NULL};
		    execv(args[0], args);
	    }
    }
    // Wait for child process to complete before resuming parent process
    wait(NULL); 
    // Restoring the privilege by using setuid(oldid)
    status = setuid(real);
    // Check for the status of setuid, if it is negative, the setuid command failed 
    if (status < 0) {
            perror("ERROR on setuid");
    }
    return 0; 
} 
