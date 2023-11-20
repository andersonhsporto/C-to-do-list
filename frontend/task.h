#ifndef TASK_H
#define TASK_H

#include <gtk/gtk.h>

typedef struct tasks
{
	GtkWidget *label;
	GtkWidget *delete_button;
	struct tasks *next;
} Tasks;

void add_task(Tasks **head, GtkWidget *label, GtkWidget *delete_button);

#endif