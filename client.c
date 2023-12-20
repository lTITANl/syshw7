#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define PORT 3000
#define BUFFER_SIZE 1024

void send_request(int server_socket, const char *request) {
    // 서버로 요청 전송
    write(server_socket, request, strlen(request) + 1);

    // 서버로부터 응답 받기
    char reply[BUFFER_SIZE];
    read(server_socket, reply, sizeof(reply));

    // 결과 출력
    printf("Server reply: %s\n", reply);

    // 결과 파일에 저장
    FILE *result_file = fopen("result.dat", "a");
    if (result_file == NULL) {
        perror("Error opening result file");
        exit(EXIT_FAILURE);
    }
    fprintf(result_file, "%s\n", reply);
    fclose(result_file);
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr;

    // 소켓 생성
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // 서버에 연결
    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error connecting to server");
        exit(EXIT_FAILURE);
    }

    // 클라이언트 요청 예시: SET key1 value1
    send_request(server_socket, "SET key1 value1");

    // 클라이언트 요청 예시: GET key1
    send_request(server_socket, "GET key1");

    // 클라이언트 요청 예시: SET key2 value2
    send_request(server_socket, "SET key2 value2");

    // 클라이언트 요청 예시: GET key2
    send_request(server_socket, "GET key2");

    // 소켓 닫기
    close(server_socket);

    return 0;
}
