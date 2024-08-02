#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <iostream>

int main() {
    int server_socket;
    int client_socket;
    struct sockaddr_un server_addr;
    struct sockaddr_un client_addr;

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, "../../socket/my_socket");

    unlink(server_addr.sun_path); // Удаляем старый сокет, если он существует
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_socket, 5);
    std::cout << "Server is listening..." << std::endl;

    while (true) {
        socklen_t clen = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &clen);
        if (client_socket < 0) {
            perror("Accept failed");
            continue; // Ignore this error and continue accepting new clients
        }

        int summ;
        while (true) {
            int bytes_read = read(client_socket, &summ, sizeof(summ));
            if (bytes_read < 0) {
                perror("Read failed");
                break; // If read failed, exit the loop and wait for a new client
            } else if (bytes_read == 0) {
                std::cout << "Connection closed by client. Receiving will be continued when client will work." << std::endl;
                break; // The client has disconnected
            } else {
                if (summ>99 && summ%32==0){
                    std::cout << "Received sum: " << summ << std::endl;
                } 
                else { 
                    std::cout << "Error (sum < 100 or sum not divisible with 32), summ = "<< summ <<std::endl;
                }
            }
        }
        close(client_socket);
    }

    close(server_socket);
    exit(0);
}