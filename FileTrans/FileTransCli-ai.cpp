#include <iostream>
#include <fstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <cstdlib>

#define PORT 8888

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <filename>" << std::endl;
        return 1;
    }

    // 创建套接字
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Error: Could not create socket" << std::endl;
        return 1;
    }

    // 设置服务器地址
    struct sockaddr_in server_address;
    std::memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) {
        std::cerr << "Error: Invalid address" << std::endl;
        return 1;
    }

    // 连接到服务器
    if (connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error: Could not connect to server" << std::endl;
        sleep(5000);
        return 1;
    }

    // 打开文件并读取内容
    std::string filename = argv[2];
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << std::endl;
        return 1;
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    char* buffer = new char[size];
    if (!file.read(buffer, size)) {
        std::cerr << "Error: Could not read file: " << filename << std::endl;
        return 1;
    }

    // 将文件发送到服务器
    if (send(sock, buffer, size, 0) != size) {
        std::cerr << "Error: Could not send file to server" << std::endl;
        return 1;
    }

    // 关闭套接字和文件
    close(sock);
    file.close();
    delete[] buffer;

    std::cout << "File " << filename << " sent to server" << std::endl;
    return 0;
}