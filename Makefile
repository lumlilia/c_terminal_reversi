NAME	= reversi
OBJS	= main.o reversi_base.o reversi_npc.o

.PHONY: all
all: $(NAME)

$(NAME): $(OBJS)
	gcc $(OBJS) -o $(NAME)

main.o: main.c
	gcc -c main.c
reversi_base.o: reversi_base.c
	gcc -c reversi_base.c
reversi_npc.o: reversi_npc.c
	gcc -c reversi_npc.c

.PHONY: clean
clean:
	$(RM) *.o

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean all

