// 1:1 client-server socket program.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

// for handling alphabet cases
#include <ctype.h>

#define BUF_SIZE    10000
#define PORT        4000

int main(int argc, char *argv[]) {
    struct sockaddr_in sock_addr;       // 서버 소켓 생성을 위해 필요한 정보를 포함하는 구조체
    socklen_t sock_len = sizeof(sock_addr);

    // socket file description  
    int listen_sockfd;                  // 클라이언트와의 연결을 대기하기 위한 서버의 file desciptor
    int connect_sockfd;                 // 클라이언트와 연결될 때, 통신에서 해당 클라이언트를 특정하기 위한 file descriptor

    int rbyte = 0, wbyte = 0;           // 각각 클라이언트로부터 받은 크기와, 클라이언트로 보낸 데이터의 크기

    char *buf = malloc(sizeof(char) * BUF_SIZE);   // 통신 중 데이터를 저장하기 위한 버퍼
    
    if ((listen_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        return 1;
    }

    // set info of listening socket to IPv4 address and port number of server
    memset((void *)&sock_addr, 0, sock_len);

    // 서버에서 사용할 IP 주소 방식, IP 주소, 포트 번호를
    // sockaddr_in 구조체에 저장한다.
    sock_addr.sin_family = AF_INET;                // IP 주소 방식 -> IPv4는 AF_INET 상수 사용
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY); // IP 주소
    sock_addr.sin_port = htons(PORT);              // 포트 번호

    // after setting info of listening socket, then apply it
    if (bind(listen_sockfd, (struct sockaddr *)&sock_addr, sock_len) < 0) {
        perror("bind() error");
        close(listen_sockfd);
        return 1;
    }

    // limit max connection at the same time
    // after calling listen() function, wait for connection request from client
    if (listen(listen_sockfd, 5) < 0) {
        perror("listen() error");
        close(listen_sockfd);
        return 1;
    }

    // if client tries to connect to server,
    // then listening socket creates connecting socket of newly connected client
    // connecting socket makes client and server communicate with each other.
    if ((connect_sockfd = accept(listen_sockfd, (struct sockaddr *)&sock_addr, &sock_len)) < 0) {
        perror("accept() error");
        close(listen_sockfd);
        return 1;
    }

    for (;;) {
        // read(), write()
        // read() : get data from opposite side which calls write() function
        // write() : send data to opposite side which calls read() function
        if ((rbyte = read(connect_sockfd, buf, BUF_SIZE)) < 0) {            
            printf("Failed to read data\n");
            break;
        }
        
        // handle buffer overflow
        if (rbyte > BUF_SIZE) {
            printf("Exceeded BUF_SIZE\n");
            break;
        }

        // handle escape condition
        if (strcmp(buf, "exit") == 0) {
            printf("bye\n");
            close(connect_sockfd);
            close(listen_sockfd);
            free(buf);
            return 0;
        }

        printf("Received data : %s\n", buf);

        // convert each alphabet letter consisted received buf(string) between lowercase and uppercase 
        // key point: we can just change the server code from simple echo program!
        for (int i = 0; i < strlen(buf); i++) {
            if (!isalpha(buf[i])) {
                continue;
            }

            if (isupper(buf[i])) {
                buf[i] = tolower(buf[i]);
                continue;
            }

            if (islower(buf[i])) {
                buf[i] = toupper(buf[i]);
            }
        }

        // read()에서 입력받은 문자열을 그대로 클라이언트에 전송
        if ((wbyte = write(connect_sockfd, buf, BUF_SIZE)) < 0) {
            printf("Failed to write data\n");
            break;
        }
        printf("Sent data : %s\n", buf);
    }
    
    close(connect_sockfd);
    close(listen_sockfd);
    free(buf);
    return 1;
}