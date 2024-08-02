#include <iostream>
#include <thread>
#include <string>
#include <regex>
#include <mutex>
#include <shared_mutex>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <algorithm>

std::regex digits_regex("^[0-9]+$");

class ThreadSafeBuffer {
public: 
    void write(std::string str) { 
        std::unique_lock lock(mutex_); 
        value_ = str; 
    } 
    
    std::string get() const { 
        std::shared_lock lock(mutex_);
        return value_; 
    } 
    
private:
    mutable std::shared_mutex mutex_; 
    std::string value_ = ""; 
}; 

void f(ThreadSafeBuffer&); 
void f2(ThreadSafeBuffer&, int& server_socket); 

int main() { 
    int server_socket; 
    struct sockaddr_un server_addr; 

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0); 
    if (server_socket < 0) { 
        perror("Socket creation failed");
        exit(1);
    } 

    server_addr.sun_family = AF_UNIX; 
    strcpy(server_addr.sun_path, "../../socket/my_socket"); 

    int connection_result = connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connection_result == -1) { 
        perror("Connection failed"); 
        exit(1); 
    } 

    ThreadSafeBuffer buf; 

    while (true) { 
        std::thread th1(f, std::ref(buf)); 
        th1.join(); 
        std::thread th2(f2, std::ref(buf), std::ref(server_socket)); 
        th2.join();
    } 

    close(server_socket); 
    exit(0);
} 

void f(ThreadSafeBuffer& buf){ 
    // Get input from user with validation 
    std::string input;
    std::cout << "Enter string that contains only numbers, length <= 64" << std::endl; 
    std::cin >> input; 

    while (!std::regex_match(input, digits_regex) || input.length() > 64) { 
        std::cout << "String doesn't match conditions, please enter new string: "; 
        std::cin >> input; 
    } 

    std::sort(input.begin(), input.end()); 
    std::reverse(input.begin(), input.end());

    for (int i = 0; i < input.length(); i++) { 
        if ((input[i] - '0') % 2 == 0) { 
            input.replace(i, 1, "KB"); 
            i++;
        }
    } 

    buf.write(input); 
}

void f2(ThreadSafeBuffer& buf, int& server_socket) {
    std::string str = buf.get(); 
    buf.write(""); 
    int summ = 0; 

    for (char ch : str) { 
        if (isdigit(ch)) { 
            summ += (ch - '0');
        } 
    } 
    
    std::cout << "Sum calculated: " << summ << std::endl; 
    write(server_socket, &summ, sizeof(summ)); 
}