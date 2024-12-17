CC		= gcc
NAME	= reversi
OBJS	= main.o term_mode_edit.o reversi_base.o check_input.o reversi_npc.o title.o


.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $(NAME)
%.o: %.c
	$(CC) -c $<


.PHONY: clean fclean re
clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME)

re: fclean all

