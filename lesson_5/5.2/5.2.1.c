#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 12345
#define BUF_SIZE 1024

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    char message[BUF_SIZE];

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка при привязке сокета");
        exit(EXIT_FAILURE);
    }

    printf("Сервер запущен и ожидает подключения...\n");

    while (1) {
        socklen_t client_len = sizeof(client_addr);
        ssize_t bytes_received = recvfrom(server_socket, message, BUF_SIZE, 0, (struct sockaddr*)&client_addr, &client_len);
        if (bytes_received < 0) {
            perror("Ошибка при получении сообщения");
            break;
        }

        printf("Получено от клиента 1: %s", message);

        if (strncmp(message, "exit", 4) == 0)
            break;

        memset(message, 0, BUF_SIZE);

        printf("Введите сообщение для клиента 1 (введите 'exit' для выхода):\n");

        fgets(message, BUF_SIZE, stdin);

        sendto(server_socket, message, strlen(message), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
    }

    close(server_socket);
    return 0;
}
