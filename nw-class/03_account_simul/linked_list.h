#pragma once

#ifndef LINKED_LIST
#define LINKED_LIST
#endif

#include "defines.h"

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

HEAD* createLinkedList();

int getSize(HEAD *head);

void insertFirstNode(HEAD *head, Account account);

void insertMiddleNode(HEAD *head, int idx, Account account);

void insertLastNode(HEAD *head, Account account);

void deleteLinkedList(HEAD *head, int idx);

Account searchAccountByID(HEAD *head, char id[]);

void printLinkedList(HEAD *head);

void freeLinkedList(HEAD *head);

Account createAccount(char id[], char password[]);

void testLinkedList();

void initLinkedList(HEAD *head);