// 1:1 client-server socket program.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE    10000
#define PORT        4000
#define IP          "127.0.0.1"

int main(int argc, char *argv[]) {
    struct sockaddr_in sock_addr;       // 연결할 서버의 정보를 포함하는 구조체


    int connect_sockfd;                 // 서버와의 통신을 위한 클라이언트 소켓
    int wbyte = 0, rbyte = 0;           // 각각 클라이언트로 보낸 데이터의 크기와, 클라이언트에서 받은 데이터의 크기
    
    char *buf = (char *)malloc(sizeof(char) * BUF_SIZE);    // 통신 중 데이터를 저장하기 위한 버퍼

    if ((connect_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket error");
        free(buf);
        return 1;
    }

    // 클라이언트에서 연결할 서버의 IP 주소 방식, IP 주소, 포트 번호를
    // sockaddr_in 구조체에 저장한다.
    sock_addr.sin_family = AF_INET;                 // IP 주소 방식 -> IPv4는 AF_INET 상수 사용
    sock_addr.sin_addr.s_addr = inet_addr(IP);      // IP 주소
    sock_addr.sin_port = htons(PORT);               // 포트 번호

    // 서버에 연결을 시도한다.
    if (connect(connect_sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        perror("connect error");
        close(connect_sockfd);
        free(buf);
        return 1;
    }

    for (;;) {
        scanf("%[^\n]s", buf);

        // read(), write()
        // read() : get data from opposite side which calls write() function
        // write() : send data to opposite side which calls read() function        
        if ((wbyte = write(connect_sockfd, buf, BUF_SIZE)) < 0) {
            printf("Failed to write data\n");
            close(connect_sockfd);
            free(buf);
            return -1;        
        }

        // handle escape condition
        if (strcmp(buf, "exit") == 0) {
            printf("bye\n");
            break;
        }
        printf("Sent data : %s\n", buf);

        if ((wbyte = read(connect_sockfd, buf, BUF_SIZE)) < 0) {
            printf("Failed to read data\n");
            break;
        }
        printf("Received data : %s\n", buf);

        // scanf 함수로 stdin 버퍼에 공백 문자를 입력함으로써,
        // stdin 버퍼를 비워 다음 반복에서 공백 포함하여 scanf 함수로 입력할 수 있게 한다.
        scanf("\n");
    }

    close(connect_sockfd);
    free(buf);
    return 0;
}