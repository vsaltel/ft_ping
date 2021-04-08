CC		=	gcc
CFLAGS	+=	-Wall -Wextra #-Werror

ifdef DEBUG
	CFLAGS += -g3 -fsanitize=address
endif

SHELL	=	bash

VALGRIND_ARGS = --leak-check=full --show-leak-kinds=all --suppressions="valgrind.supp"

NAME 	=	ft_ping
LIBFT	=	libft
SRCDIR	=	srcs
INCDIR	=	includes
OBJDIR	=	objs

FILES	=	main.c				\
			ping.c				\
			socket.c			\
			recv_msg.c			\
			send_msg.c			\
			rev_dns.c			\
			conv_addr.c			\
			utils.c				\
			print.c				\
			signal.c			\
			args.c				\
			args_utils.c

SRCS	=	$(addprefix $(SRCDIR)/, $(FILES))
OBJS	=	$(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
OBJSD	=	$(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.d)

##### Colors #####
_END=\x1b[0m
_BOLD=\x1b[1m
_UNDER=\x1b[4m
_REV=\x1b[7m
_GREY=\x1b[30m
_RED=\x1b[31m
_GREEN=\x1b[32m
_YELLOW=\x1b[33m
_BLUE=\x1b[34m
_PURPLE=\x1b[35m
_CYAN=\x1b[36m
_WHITE=\x1b[37m

.PHONY: all clean fclean re norm tests $(LIBFT)


.SILENT:

all: $(NAME)

$(NAME): $(OBJS)
	@$(MAKE) -q -C $(LIBFT) || $(MAKE) -j4 -C $(LIBFT)
	@echo -e -n "\n${_BLUE}${_BOLD}[Create Executable] $(NAME)${_END}"
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L./$(LIBFT) -lft
	@echo -e "\n${_GREEN}${_BOLD}$(NAME) done.${_END}"

$(OBJDIR)/%.o: $(SRCDIR)/%.c Makefile
	@mkdir -p $(@D)
	@echo -n -e "\r\033[K${_PURPLE}${BOLD}[${NAME}] Compiling $<${_END}"
	@$(CC) $(CFLAGS) -I $(INCDIR) -I $(LIBFT)/$(INCDIR) -MMD -o $@ -c $<

libft:
	@$(MAKE) -q -C $(LIBFT) || $(MAKE) -j4 -C $(LIBFT)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -L./$(LIBFT) -lft -D_GNU_SOURCE

clean:
	@$(MAKE) -C $(LIBFT) clean
	@echo -e "${_RED}${_BOLD}Cleaning obj files...${_END}"
	@rm -f $(OBJS)
	@rm -f $(OBJSD)
	@rm -Rf $(OBJDIR)

fclean: clean
	@$(MAKE) -C $(LIBFT) fclean
	@echo -e "${_RED}${_BOLD}Cleaning project...${_END}"
	@rm -f $(NAME)
	@rm -rf $(OBJDIR)
	@rm -rf $(NAME).dSYM

re: fclean
	@$(MAKE)

norm:
	@norminette $(INCDIR) $(SRCDIR) $(LIBFT)/$(INCDIR) $(LIBFT)/$(SRCDIR) | grep "Warning\|Error" || true
	@echo "Norm done!"

valgrind: all
	\valgrind $(VALGRIND_ARGS) "./${NAME}"

-include $(OBJSD)
