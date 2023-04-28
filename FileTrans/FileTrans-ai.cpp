#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

const int BUFFER_SIZE = 1024;
const int SERVER_PORT = 8888;

int main()
{
    // 创建 socket
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket < 0)
    {
        cout << "Failed to create socket." << endl;
        return -1;
    }

    // 设置服务地址
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(SERVER_PORT);

    // 绑定 socket 和地址
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        cout << "Failed to bind socket." << endl;
        return -1;
    }

    while (1)    
    {        
        // 开始监听
        if (listen(server_socket, 5) < 0)
        {
            cout << "Failed to listen on socket." << endl;
            return -1;
        }

        cout << "Waiting for client connection..." << endl;

        // 接受客户端连接
        struct sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_size);  // 得到一个 TCP 连接
        if (client_socket < 0)
        {
            cout << "Failed to accept client connection." << endl;
            return -1;
        }

        // 接收文件名
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        int recv_size = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (recv_size < 0)
        {
            cout << "Failed to receive filename." << endl;
            return -1;
        }

        string filename = buffer;
        cout << "Receive" << endl;
        cout << filename << endl;
    }
}