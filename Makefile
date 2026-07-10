# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fmesa-or <fmesa-or@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/28 17:42:13 by fmesa-or          #+#    #+#              #
#    Updated: 2026/07/10 17:19:35 by fmesa-or         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


#Name
NAME = ircserv

#Compile
CC = c++

#Flags
CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -pedantic

#Clean
CLEAN = rm -f

#Sources
SRC_DIR = src
SRC = \
	src/dispatcher/CommandDispatcher.cpp \
	src/handlers/ChannelHandlers.cpp \
	src/handlers/HandlerRegistry.cpp \
	src/handlers/MsgHandlers.cpp \
	src/handlers/ProtocolHandlers.cpp \
	src/handlers/RegistrationHandlers.cpp \
	src/parser/Parser.cpp \
	src/replies/Replies.cpp \
	src/Client.cpp \
	src/helpers.cpp \
	src/main.cpp \
	src/Server.cpp \
	src/Channel/Channel.cpp

#Objects
OBJ_DIR = obj
#OBJ = $(SRC:.cpp=.o)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))

#Headers
HEADERS = -I ./include

#Colors
COLOR_INFO = \033[1;36m
COLOR_SUCCESS = \033[1;32m
COLOR_RESET = \033[0m
COLOR_PROGRESS = \033[1;35m
COLOR_HEADER = \033[1;35m

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CPPFLAGS) $(OBJ) $(HEADERS) -o $(NAME)
	@printf "\r\033[K$(COLOR_SUCCESS)✅ $(NAME) is ready!$(COLOR_RESET)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@printf "\r$(COLOR_INFO)Building $(NAME)... [%s]$(COLOR_RESET)" $(notdir $<)
	@$(CC) $(CPPFLAGS) -c $< -o $@ $(HEADERS)


clean:
	@printf "$(COLOR_INFO)Cleaning object files...$(COLOR_RESET)"
	@$(CLEAN) -r $(OBJ_DIR)
	@printf "\r$(COLOR_SUCCESS)✅ Object files cleaned successfully!$(COLOR_RESET)\n"

fclean: clean
	@printf "$(COLOR_INFO)Deleting $(NAME)...$(COLOR_RESET)"
	@$(CLEAN) $(NAME)
	@printf "\r$(COLOR_SUCCESS)✅ $(NAME) deleted successfully!$(COLOR_RESET)\n"

re: fclean $(NAME)

.PHONY: all clean fclean re
