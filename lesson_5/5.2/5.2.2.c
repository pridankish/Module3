#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUF_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char message[BUF_SIZE];

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Ошибка при преобразовании IP адреса");
        exit(EXIT_FAILURE);
    }

    printf("Введите сообщение для клиента 2 (введите 'exit' для выхода):\n");

    while (1) {
        fgets(message, BUF_SIZE, stdin);

        sendto(client_socket, message, strlen(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        if (strncmp(message, "exit", 4) == 0)
            break;

        memset(message, 0, BUF_SIZE);

        ssize_t bytes_received = recvfrom(client_socket, message, BUF_SIZE, 0, NULL, NULL);
        if (bytes_received < 0) {
            perror("Ошибка при получении сообщения");
            break;
        }

        printf("Ответ от клиента 2: %s", message);
    }

    close(client_socket);
    return 0;
}
