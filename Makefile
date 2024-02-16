CC = g++
CFLAGS = -Wall -Wextra -Werror -g 
NAME = ircserv
FILE = Server/Server.cpp Client/Client.cpp main.cpp Channel/Channel.cpp \
	Message/Message.cpp Message/Message_utils.cpp Response/Response.cpp utils/utils.cpp

all : make
	rm -rf *.dSYM

re : fclean all

make : 
	$(CC) $(CFLAGS) $(FILE) -o $(NAME)

clean:
	rm -rf $(NAME)
	rm -rf *.dSYM

fclean: clean

.PHONY: all clean