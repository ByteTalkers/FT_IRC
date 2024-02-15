# CFLAGS = -Wall -Wextra -Werror -g
CFLAGS = -w
NAME = test
FILE = ../Server/Server.cpp \
	../Client/Client.cpp \
	../Message/Message.cpp \
	../utils/utils.cpp \
	../Channel/Channel.cpp \
	../Response/Response.cpp \

 TEST = messageTest.cpp
# TEST = responseTest.cpp

all : make
	rm -rf *.dSYM

re : fclean all

make : 
	$(CXX) $(CFLAGS) $(FILE) $(TEST) -o $(NAME)

clean:
	rm -rf $(NAME)
	rm -rf *.dSYM

fclean: clean

.PHONY: all clean