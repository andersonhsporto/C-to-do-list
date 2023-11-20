#include "gui.h"
#include "task.h"

GtkEntryBuffer *buffer_name;
GtkEntryBuffer *buffer_content;
Node *list = NULL;
Tasks *tasks = NULL;

// Função callback para remover todas as tarefas
void remove_all_tasks(GtkButton *button, gpointer user_data)
{
	Tasks *temp = tasks;
	if (temp == NULL)
	{
		return;
	}
	while (tasks != NULL)
	{
		gtk_widget_destroy(temp->label);
		gtk_widget_destroy(temp->delete_button);
		tasks = temp->next;
		free(temp);
		temp = tasks;
	}
	free_list(list);
	tasks = NULL;
	list = NULL;
}

// Função callback para obter todas as tarefas
void get_all_tasks(GtkButton *button, struct gambiarra *g)
{
	do_get_request(&list);
	print_list(list);

	// adicionar as tarefas a janela principal
	Node *current = list;
	int rowCount = 3;
	while (current != NULL)
	{
		GtkWidget *label = gtk_label_new(g_strdup_printf("%s", current->text));
		// gtk_container_add(GTK_CONTAINER(g->wid), label);

		// Adicionar o rótulo ao layout de grade
		gtk_grid_attach(GTK_GRID(g->wid2), label, 0, rowCount, 2, 1);

		// Adicionar um botão "Deletar" para cada tarefa existente
		GtkWidget *delete_button = gtk_button_new_with_label("Deletar");
		g_signal_connect(delete_button, "clicked", G_CALLBACK(NULL), label);
		gtk_grid_attach(GTK_GRID(g->wid2), delete_button, 2, rowCount, 1, 1);

		add_task(&tasks, label, delete_button);
		rowCount++;
		current = current->next;
	}
	gtk_widget_show_all(g->wid);
}

// Função callback para adicionar uma tarefa à lista
void on_add_button_clicked(GtkButton *button, gpointer user_data)
{
	GtkWidget *name_entry = GTK_WIDGET(user_data);
	GtkWidget *content_entry = GTK_WIDGET(g_object_get_data(G_OBJECT(button), "content_entry"));

	// Obter o texto dos campos de entrada
	const char *task_name = gtk_entry_buffer_get_text(buffer_name);
	const char *task_content = gtk_entry_buffer_get_text(buffer_content);

	// Chamar a função do_post_request
	do_post_request(task_name, task_content);

	// Limpar os campos de entrada
	gtk_entry_buffer_set_text(buffer_name, "", -1);
	gtk_entry_buffer_set_text(buffer_content, "", -1);
}

// Função callback para ativar a aplicação
void activate(GtkApplication *app, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *grid;
	GtkWidget *name_entry;
	GtkWidget *content_entry;
	GtkWidget *add_button;
	GtkWidget *name_label;
	GtkWidget *content_label;
	struct gambiarra *gambiarra = malloc(sizeof(struct gambiarra));

	// Criar a janela principal
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "To-Do List Safada Em  C");
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);

	// Criar um layout de grade
	grid = gtk_grid_new();
	gtk_container_add(GTK_CONTAINER(window), grid);

	// Criar rótulos para os campos de entrada
	name_label = gtk_label_new("Nome da Tarefa:");
	content_label = gtk_label_new("Conteúdo da Tarefa:");

	// Criar um campo de entrada para o nome da tarefa
	name_entry = gtk_entry_new();
	buffer_name = gtk_entry_get_buffer(GTK_ENTRY(name_entry));
	gtk_grid_attach(GTK_GRID(grid), name_label, 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), name_entry, 1, 0, 1, 1);

	// Criar um campo de entrada para o conteúdo da tarefa
	content_entry = gtk_entry_new();
	buffer_content = gtk_entry_get_buffer(GTK_ENTRY(content_entry));
	gtk_grid_attach(GTK_GRID(grid), content_label, 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), content_entry, 1, 1, 1, 1);

	// Criar um botão "Adicionar"
	add_button = gtk_button_new_with_label("Adicionar");
	g_signal_connect(add_button, "clicked", G_CALLBACK(on_add_button_clicked), name_entry);
	g_signal_connect(add_button, "clicked", G_CALLBACK(remove_all_tasks), grid);

	gambiarra->wid = window;
	gambiarra->wid2 = grid;

	// Associar o campo de conteúdo à função callback
	g_object_set_data(G_OBJECT(add_button), "content_entry", content_entry);
	gtk_grid_attach(GTK_GRID(grid), add_button, 0, 2, 2, 1);

	g_signal_connect(add_button, "clicked", G_CALLBACK(get_all_tasks), gambiarra);
	// Fazer uma solicitação GET
	do_get_request(&list);

	// adicionar as tarefas a janela principal
	Node *current = list;
	int rowCount = 3;
	while (current != NULL)
	{
		GtkWidget *label = gtk_label_new(g_strdup_printf("%s", current->text));

		// Adicionar o rótulo ao layout de grade
		gtk_grid_attach(GTK_GRID(grid), label, 0, rowCount, 2, 1);

		// Adicionar um botão "Deletar" para cada tarefa existente
		GtkWidget *delete_button = gtk_button_new_with_label("Deletar");
		g_signal_connect(delete_button, "clicked", G_CALLBACK(NULL), label);
		gtk_grid_attach(GTK_GRID(grid), delete_button, 2, rowCount, 1, 1);

		add_task(&tasks, label, delete_button);

		rowCount++;
		current = current->next;
	}
	gtk_widget_show_all(window);
}
