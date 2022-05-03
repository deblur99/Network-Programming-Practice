#include "linked_list.h"

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

Account searchAccountByID(HEAD *head, char id[]) {
    Account ret = {"", ""};

    NODE *temp = head->link;

    while (temp != NULL) {
        if (strcmp(temp->account.id, id) == 0) {
            break;
        }
        temp = temp->link;
    }

    if (temp != NULL) {
        ret = temp->account;
    }

    return ret;
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
    insertLastNode(head, createAccount("amybgyhouse", "19871023"));
    insertLastNode(head, createAccount("socketpgm", "assnmnt2323"));
    insertFirstNode(head, createAccount("dankook", "univ!"));
    
    printLinkedList(head);

    Account res = searchAccountByID(head, "amybgyhouse");
    printf("Found : ID - %s, PW - %s\n", res.id, res.password);

    freeLinkedList(head);
}

void initLinkedList(HEAD *head) {
    insertFirstNode(head, createAccount("deblur99", "1Q2W3E4R!"));
    insertLastNode(head, createAccount("amybgyhouse", "19871023"));
    insertLastNode(head, createAccount("mynameeee", "1q2w3e4r!"));
}