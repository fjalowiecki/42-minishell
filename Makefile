# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fgrabows <fgrabows@student.42warsaw.pl>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/29 10:36:34 by fjalowie          #+#    #+#              #
#    Updated: 2024/10/17 20:19:29 by fgrabows         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= minishell

SRCS	= src/main.c \
			src/envp.c \
			src/syntax_check.c \
			src/sanitation.c \
			src/utils.c \
			src/tokens00.c \
			src/tokens01.c \
			src/error.c \
			src/tokenizer.c \
			src/helpers.c \
			src/get_next_line.c \
			src/cmds.c \
			src/execution.c \
			src/fd_handlers.c \
			src/cmds_redirs.c \
			src/dollar_sign.c \
			src/builtins.c \
			src/blt_echo.c \
			src/blt_cd.c \
			src/blt_pwd.c \
			src/blt_export.c \
			src/blt_unset.c \

OBJDIR	= .obj
OBJS	= $(SRCS:src/%.c=$(OBJDIR)/%.o)

CC		= cc
CFLAGS	= -I includes/ -g
# -Wall -Wextra -Werror 
LDFLAGS = -lreadline

all:	$(NAME)

$(NAME): $(OBJS)
	@$(MAKE) --no-print-directory -C ./libft
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) libft/libft.a $(LDFLAGS)
	@echo "Build of $(NAME) completed."

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c -o $@ $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean:
	@$(MAKE) --no-print-directory clean -C ./libft
	@rm -f $(OBJS)
	@echo "Clean of ${NAME} completed."

fclean: clean
	@$(MAKE) --no-print-directory fclean -C ./libft
	@rm -f $(NAME)
	@rm -rf $(OBJDIR)
	@echo "Full clean of ${NAME} completed."

re: fclean all
	@echo "Rebuild of ${NAME} completed."

.PHONY: all clean fclean re