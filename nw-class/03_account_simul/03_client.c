// 1:1 client-server socket program.
#include "defines.h"

int main(int argc, char *argv[]) {
    struct sockaddr_in sock_addr;       

    int connect_sockfd;                 
    int wbyte = 0, rbyte = 0;           
    
    char *buf = (char *)malloc(sizeof(char) * BUF_SIZE);

    if ((connect_sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        perror("socket error");
        free(buf);
        return 1;
    }

    sock_addr.sin_family = AF_INET;                 
    sock_addr.sin_addr.s_addr = inet_addr(IP);      
    sock_addr.sin_port = htons(PORT);               

    if (connect(connect_sockfd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
        perror("connect error");
        close(connect_sockfd);
        free(buf);
        return 1;
    }

    for (;;) {
        // step 1) get user ID
        printf("Enter user ID to sign in: ");
        scanf("%s", buf); // gets until space (' ') letter
        
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
        printf("Sent id : %s\n", buf);

        if ((rbyte = read(connect_sockfd, buf, BUF_SIZE)) < 0) {
            printf("Failed to read data\n");
            break;
        }
        printf("Received branch : %s\n", buf);
        
        // branch: whether id exists on server or not
        // if it exists, server acquires user password
        // it it does not, server acquires user password as well to sign up
        if (strcmp(buf, "signin") != 0 && strcmp(buf, "signup") != 0) {
            break;
        }

        if (strcmp(buf, "signin") == 0) {
            printf("Enter the password to sign in : ");
        } else if (strcmp(buf, "signup") == 0) {
            printf("Enter the password to sign up : ");
        }
        // enter password
        scanf("%s", buf);

        if ((wbyte = write(connect_sockfd, buf, BUF_SIZE)) < 0) {
            printf("Failed to write data\n");
            close(connect_sockfd);
            free(buf);
            return -1;        
        }
        printf("Sent password : %s\n", buf);

        if ((rbyte = read(connect_sockfd, buf, BUF_SIZE)) < 0) {
            printf("Failed to read data\n");
            break;
        }
        printf("Received result : %s\n", buf);
    }

    close(connect_sockfd);
    free(buf);
    return 0;
}