#ifndef REST_H
#define REST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 3000
// Estrutura para representar um item da lista
typedef struct Node
{
	int id;
	char *name;
	char *text;
	struct Node *next;
} Node;

void do_post_request(const char *name, const char *text);
void do_get_request(Node **list);
void free_list(Node *head);
void print_list(Node *head);

#endif
