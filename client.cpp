#include<iostream>
#include<cstring>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<cstring>
using namespace std;

#define PORT 8080

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        cerr << "Socket creation failed" << endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Connection failed" << endl;
        return -1;
    }

    string username, from, to, message;
    cout << "Enter your username: ";
    cin >> username;
    cin.ignore();
    cout << "From: " << username << endl;
    cout << "To: ";
    cin >> to;
    cin.ignore();

    while (true) {
        cout << "Enter message: ";
        getline(cin, message);
        if (message == "exit") break;
        send(clientSocket, message.c_str(), message.size(), 0);
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cerr << "Server disconnected" << endl;
            break;
        }
        buffer[bytesReceived] = '\0'; 
        cout << "Server response: " << buffer << endl;
    }
    close(clientSocket);
    return 0;
}
