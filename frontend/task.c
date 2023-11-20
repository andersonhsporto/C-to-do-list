#include "task.h"

Tasks *get_last_task(Tasks *head)
{
	Tasks *temp = head;

	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	return temp;
}

void add_task(Tasks **head, GtkWidget *label, GtkWidget *delete_button)
{
	Tasks *temp;

	if (*head == NULL)
	{
		*head = (Tasks *)malloc(sizeof(Tasks));
		(*head)->label = label;
		(*head)->delete_button = delete_button;
		(*head)->next = NULL;
	}
	else
	{
		temp = get_last_task(*head);
		temp->next = (Tasks *)malloc(sizeof(Tasks));
		temp->next->label = label;
		temp->next->delete_button = delete_button;
		temp->next->next = NULL;
	}
}