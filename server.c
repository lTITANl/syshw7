#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 3000
#define BUFFER_SIZE 1024

void handle_set(int client_socket, char *key, char *value) {
    // 처리된 결과를 클라이언트에 전송
    char reply[BUFFER_SIZE];
    sprintf(reply, "SET key='%s' value='%s' successful. Value size: %lu", key, value, strlen(value));

    write(client_socket, reply, strlen(reply) + 1);
}

void handle_get(int client_socket, char *key) {
    // 처리된 결과를 클라이언트에 전송 (임의로 "sample_value"로 가정)
    char reply[BUFFER_SIZE];
    sprintf(reply, "GET key='%s' successful. Value: sample_value", key);

    write(client_socket, reply, strlen(reply) + 1);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // 소켓 생성
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // 소켓에 주소 할당
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // 연결 대기
    if (listen(server_socket, 5) == -1) {
        perror("Error listening for connections");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        // 클라이언트 연결 수락
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        // 클라이언트로부터 요청 수신
        read(client_socket, buffer, sizeof(buffer));

        // 요청 파싱
        char *token = strtok(buffer, " ");
        char *operation = token;
        char *key = strtok(NULL, " ");
        char *value = strtok(NULL, " ");

        // 요청 처리
        if (strcmp(operation, "SET") == 0) {
            handle_set(client_socket, key, value);
        } else if (strcmp(operation, "GET") == 0) {
            handle_get(client_socket, key);
        } else {
            // 잘못된 요청 처리
            char reply[BUFFER_SIZE] = "Invalid request";
            write(client_socket, reply, strlen(reply) + 1);
        }

        // 연결 종료
        close(client_socket);
    }

    // 소켓 닫기
    close(server_socket);

    return 0;
}
