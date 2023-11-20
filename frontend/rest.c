#include "rest.h"

void do_post_request(const char *name, const char *text)
{
	int sockfd;
	struct sockaddr_in server_addr;

	if (name == NULL || text == NULL)
	{
		return;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("Erro ao criar o socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0)
	{
		perror("Erro ao converter endereço IP");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("Erro ao conectar ao servidor");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	char json[256];
	snprintf(json, sizeof(json), "{ \"name\": \"%s\", \"text\": \"%s\" }", name, text);

	char request[1024];
	sprintf(request, "POST /task HTTP/1.1\r\n"
					 "Host: %s:%d\r\n"
					 "Content-Type: application/json\r\n"
					 "Content-Length: %zu\r\n"
					 "\r\n"
					 "%s",
			SERVER_IP, SERVER_PORT, strlen(json), json);

	if (send(sockfd, request, strlen(request), 0) == -1)
	{
		perror("Erro ao enviar a solicitação HTTP");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	char response[1024];
	ssize_t bytes_received = recv(sockfd, response, sizeof(response) - 1, 0);
	if (bytes_received == -1)
	{
		perror("Erro ao receber a resposta do servidor");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	response[bytes_received] = '\0';

	// Imprimir a resposta do servidor
	// printf("Resposta do servidor:\n%s\n", response);

	close(sockfd);
}

void print_list(Node *head)
{
	Node *current = head;

	while (current != NULL)
	{
		printf("ID: %d, Name: %s, Text: %s\n", current->id, current->name, current->text);
		current = current->next;
	}
}

Node *get_last_node(Node *head)
{
	Node *temp = head;

	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	return temp;
}

void push(Node **head, int id, const char *name, const char *text)
{
	Node *temp;

	if (*head == NULL)
	{
		*head = (Node *)malloc(sizeof(Node));
		(*head)->id = id;
		(*head)->name = strdup(name);
		(*head)->text = strdup(text);
		(*head)->next = NULL;
	}
	else
	{
		temp = get_last_node(*head);
		temp->next = (Node *)malloc(sizeof(Node));
		temp->next->id = id;
		temp->next->name = strdup(name);
		temp->next->text = strdup(text);
		temp->next->next = NULL;
	}
}

void free_list(Node *head)
{
	Node *temp = head;
	while (head != NULL)
	{
		head = head->next;
		free(temp->name);
		free(temp->text);
		free(temp);
		temp = head;
	}
}

void do_get_request(Node **list)
{
	int sockfd;
	struct sockaddr_in server_addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("Erro ao criar o socket");
		exit(EXIT_FAILURE);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);			// Porta do servidor
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // Endereço do servidor

	if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
	{
		perror("Erro ao conectar ao servidor");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	const char *get_request = "GET /task HTTP/1.1\r\nHost: localhost\r\n\r\n";

	if (send(sockfd, get_request, strlen(get_request), 0) == -1)
	{
		perror("Erro ao enviar a solicitação GET");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	char buffer[4096];
	ssize_t bytes_received;
	char *data_start;
	int content_length = 0;
	int content_received = 0;

	while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0)
	{
		buffer[bytes_received] = '\0';

		// Encontrar o início dos dados JSON
		if ((data_start = strstr(buffer, "\r\n\r\n")) != NULL)
		{
			data_start += 4; // Passar para além dos cabeçalhos
			content_received = bytes_received - (data_start - buffer);
			break;
		}
	}

	// Processar os dados JSON recebidos e preencher a lista
	if (content_received > 0)
	{
		char *json_data = data_start;
		// Parse JSON e adicionar à lista ligada
		// (Neste exemplo, assume-se que o formato do JSON está fixo)
		int id;
		char name[256];
		char text[256];

		for (int i = 0; i < content_received; i++)
		{
			if (json_data[i] == '{')
			{
				sscanf(json_data + i, "{ \"id\": %d, \"name\": \"%[^\"]\", \"text\": \"%[^\"]\" }", &id, name, text);
				push(list, id, name, text);
			}
		}
	}
	close(sockfd);
}
