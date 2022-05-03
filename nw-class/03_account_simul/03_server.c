// 1:1 client-server socket program.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUF_SIZE            10000
#define PORT                4000

#define MAX_ACCOUNTS        50

#define ID_LENGTH           20
#define PW_LENGTH           40

// 연결리스트로 구현해보자!
typedef struct _Account {
    char id[ID_LENGTH];
    char password[PW_LENGTH];
}Account;

typedef struct _NODE {
    Account account;
    struct _NODE *link;
}NODE;

typedef struct _HEAD {
    NODE *link;
}HEAD;

HEAD* createLinkedList() {
    HEAD *head = (HEAD *)malloc(sizeof(HEAD));
    return head;
}

int getSize(HEAD *head) {
    int size = 0;
    NODE *temp = head->link;

    while (temp != NULL) {
        temp = temp->link;
        size++;
    }

    return size;
}

void insertFirstNode(HEAD *head, Account account) {
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    strcpy(newNode->account.id, account.id);
    strcpy(newNode->account.password, account.password);
    newNode->link = head->link;
    head->link = newNode;
}

void insertMiddleNode(HEAD *head, int idx, Account account) {
    // check input index is out of range
    if (idx >= getSize(head)) {
        printf("Error : index out of range\n");
        return;
    }

    NODE *temp = head->link;
    for (int i = 0; i < idx - 1; i++) {
        temp = temp->link;
    }

    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    strcpy(newNode->account.id, account.id);
    strcpy(newNode->account.password, account.password);
    newNode->link = temp->link;
    temp->link = newNode;    
}

void insertLastNode(HEAD *head, Account account) {
    NODE *temp = head->link;

    if (getSize(head) <= 1) {
        return insertFirstNode(head, account);
    }

    while (temp->link != NULL) {
        temp = temp->link;
    }

    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    strcpy(newNode->account.id, account.id);
    strcpy(newNode->account.password, account.password);
    newNode->link = NULL;
    temp->link = newNode;
}

void deleteLinkedList(HEAD *head, int idx) {
    // check input index is out of range
    if (idx >= getSize(head)) {
        printf("Error : index out of range\n");
        return;
    }

    NODE *temp = head->link;
    NODE *pre;

    for (int i = 0; i < idx; i++) {
        pre = temp;
        temp = temp->link;
    }

    pre->link = temp->link;
    temp->link = NULL;

    free(temp);
}

void printLinkedList(HEAD *head) {
    NODE *temp = head->link;

    while (temp != NULL) {
        printf("ID : %s, Password : %s\n", temp->account.id, temp->account.password);
        temp = temp->link;
    }
}

void freeLinkedList(HEAD *head) {
    NODE *temp = head->link;
    NODE *pre = temp;
    
    while (temp != NULL) {
        temp = temp->link;
        free(pre);
        pre = temp;
    }

    free(head);
}

Account createAccount(char id[], char password[]) {
    Account account = {"", ""};

    if (strlen(id) > ID_LENGTH || strlen(password) > PW_LENGTH) {
        return account;
    }

    strcpy(account.id, id);
    strcpy(account.password, password);
    
    return account;
}

// debug
void testLinkedList() {
    HEAD *head = createLinkedList();
    insertFirstNode(head, createAccount("deblur99", "1Q2W3E4R!"));
    insertMiddleNode(head, 2, createAccount("mynameeee", "1q2w3e4r!"));
    insertMiddleNode(head, 3, createAccount("mynameeee", "1q2w3e4r!"));
    insertMiddleNode(head, 1, createAccount("mynameeee", "1q2w3e4r!"));
    insertMiddleNode(head, 0, createAccount("mynameeee", "1q2w3e4r!"));
    insertLastNode(head, createAccount("amybgyhouse", "password"));
    insertLastNode(head, createAccount("socketpgm", "assnmnt2323"));
    insertFirstNode(head, createAccount("dankook", "univ!"));
    
    printLinkedList(head);

    freeLinkedList(head);
}

int checkExistedAccount(char id[]);

int checkPasswordIsCorrect(char password[]);

void signupNewAccount(char id[], char password[]);

int main(int argc, char *argv[]) {
    testLinkedList(); // debug
    return 0;


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

int checkExistedAccount(char id[]) {
    // TO DO : implementation this
    // while (!(strncmp(id, "", 1) == 0)) {
    //     for (int i = 0; i < strlen(id); i++) {
    //         if (id[i] != )
    //     }
    // }
    
    return 0;
}

int checkPasswordIsCorrect(char password[]) {

    return 0;

}

void signupNewAccount(char id[], char password[]) {

}