#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "rest.h"

// Estrutura para armazenar informações sobre uma tarefa
typedef struct
{
	GtkWidget *task_box;	  // Container para a tarefa
	GtkWidget *title_label;	  // Rótulo de título da tarefa
	GtkWidget *content_label; // Rótulo de conteúdo da tarefa
	GtkWidget *button;		  // Botão de exclusão
} TaskInfo;

// Estrutura para armazenar informações gerais
typedef struct
{
	GtkEntry *title_entry;
	GtkEntry *content_entry;
	GtkWidget *list;
	GList *tasks; // Lista de tarefas
} TaskData;

struct gambiarra
{
	GtkWidget *wid;
	GtkWidget *wid2;
};

void post_task(const char *title, const char *content);
void activate(GtkApplication *app, gpointer user_data);

#endif