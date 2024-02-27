NAME = ircserv

CXX = c++ -std=c++98
CXXFLAGS = -Wall -Wextra -Werror -g #-fsanitize=address
RM = rm -f

# 소스 파일 목록
SRCS = Server/Server.cpp Server/utils.cpp \
       Client/Client.cpp main.cpp Channel/Channel.cpp \
       Message/Message.cpp Response/Response.cpp utils/utils.cpp \
       Message/nick.cpp Message/pass.cpp Message/user.cpp Message/parsing.cpp \
       Message/who.cpp Message/mode.cpp Message/ping.cpp Message/join.cpp \
       Message/part.cpp Message/Command.cpp Message/quit.cpp Message/topic.cpp \
       Message/privmsg.cpp Message/kick.cpp Message/invite.cpp

# 객체 파일 목록을 obj 디렉토리 내에 생성
OBJS = $(SRCS:%.cpp=obj/%.o)

all : $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

obj/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean :
	$(RM) -r obj
	$(RM) -r *.dSYM

fclean : clean
	$(RM) $(NAME)

re : fclean all

.PHONY : all clean fclean re
