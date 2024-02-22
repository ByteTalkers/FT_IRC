CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -g 
NAME = ircserv
FILE = Server/Server.cpp Server/utils.cpp \
	Client/Client.cpp main.cpp Channel/Channel.cpp \
	Message/Message.cpp Response/Response.cpp utils/utils.cpp \
	Message/nick.cpp Message/pass.cpp Message/user.cpp Message/parsing.cpp \
	Message/whois.cpp Message/mode.cpp Message/ping.cpp Message/join.cpp \
	Message/part.cpp Message/Command.cpp Message/quit.cpp Message/topic.cpp \
	Message/privmsg.cpp Message/kick.cpp

all : make
	rm -rf *.dSYM

re : fclean all

make : 
	$(CXX) $(CXXFLAGS) $(FILE) -o $(NAME)

clean:
	rm -rf $(NAME)
	rm -rf *.dSYM

fclean: clean

.PHONY: all clean