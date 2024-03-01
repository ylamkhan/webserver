NAME = webserver
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3
RM = rm -rf

SRCS = Srcs/main.cpp Srcs/ConfigParser.cpp Srcs/Location.cpp Srcs/Server_Data.cpp \
Srcs/Socket.cpp Srcs/Client.cpp Srcs/httpReponse.cpp \
Srcs/post.cpp Srcs/get.cpp Srcs/delete.cpp Srcs/matching.cpp Srcs/parsing_requeste.cpp \
Srcs/types.cpp 

OBJS = $(SRCS:.cpp=.o)

all:$(NAME)

$(NAME):$(OBJS)
	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME) 

clean:
	$(RM) $(OBJS)

fclean:clean
	$(RM) $(NAME)

re:fclean all

.PHONY: fclean clean all re



# NAME = webserver
# CPP = c++
# CPPFLAGS = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3
# RM = rm -rf

# SRCS = Srcs/main.cpp Srcs/ConfigParser.cpp Srcs/Location.cpp Srcs/Server_Data.cpp \
# Srcs/Socket.cpp Srcs/Client.cpp Srcs/httpReponse.cpp Srcs/httpRequeste.cpp \
# Srcs/Webserver.cpp Srcs/httpHeader.cpp \

# OBJ_DIR = OBJECTS
# OBJS = $(patsubst Srcs/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# all: $(NAME)

# $(NAME): $(OBJS)
# 	$(CPP) $(CPPFLAGS) $(OBJS) -o $(NAME)

# $(OBJ_DIR)/%.o: Srcs/%.cpp | $(OBJ_DIR)
# 	$(CPP) $(CPPFLAGS) -c $< -o $@

# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# clean:
# 	$(RM) $(OBJ_DIR)

# fclean: clean
# 	$(RM) $(NAME)

# re: fclean all

# .PHONY: fclean clean all re
