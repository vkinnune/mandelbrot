INC=minilibx

NAME= mb
SRC = main.c

all	:$(NAME)

$(NAME)	: $(SRC)
	gcc -O3 $(SRC) -L$(INC) -lm -lmlx -lX11 -lXext -o $(NAME)
clean	:
	rm -f $(NAME) $(OBJ) *~ core *.core

re	: clean all
