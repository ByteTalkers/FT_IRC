CC = g++
CFLAGS = -Wall -Wextra -Werror -g 
NAME = ircserv
FILE = Server/Server.cpp Server/utils.cpp \
	Client/Client.cpp main.cpp Channel/Channel.cpp \
	Message/Message.cpp Response/Response.cpp utils/utils.cpp \
	Message/nick.cpp Message/pass.cpp Message/user.cpp Message/parsing.cpp Message/whois.cpp Message/mode.cpp Message/ping.cpp\
	Message/Command.cpp

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