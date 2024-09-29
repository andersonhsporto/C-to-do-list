CC = gcc
CFLAGS = -lcurl -g `pkg-config --cflags gtk+-3.0`
LDFLAGS = `pkg-config --libs gtk+-3.0`
TARGET = todo-list
SRCS = $(addprefix ./frontend/, main.c gui.c rest.c task.c)
OBJS = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o  $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(TARGET)


clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(TARGET)

re: fclean all

express:
	cd backend && npm install && npm start

run: express
	./$(TARGET)

.PHONY: all clean fclean re