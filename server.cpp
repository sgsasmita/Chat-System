#include<iostream>
#include<thread>
#include<vector>
#include<string>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<fstream>
#include<cstring>

using namespace std;

#define PORT 8080
#define MAX_CLIENTS 10

vector<int> clients; 

void handleClient(int clientSocket) {
    char buffer[1024];
    string message;
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            close(clientSocket);
            break;
        }
        buffer[bytesReceived] = '\0'; 
        message = string(buffer);
        ofstream logFile("chat_history.txt", ios::app);
        logFile << "Received: " << message << endl;
        logFile.close();
        cout << "Message: " << message << endl;
        send(clientSocket, buffer, bytesReceived, 0);
    }
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Socket creation failed" << endl;
        return -1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Binding failed" << endl;
        return -1;
    }

    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        cerr << "Listening failed" << endl;
        return -1;
    }

    cout << "Server running on port " << PORT << endl;
    while (true) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            cerr << "Accept failed" << endl;
            continue;
        }
        clients.push_back(clientSocket); 
        thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }
    return 0;
}
