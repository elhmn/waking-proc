TARGET = libwaking-proc.a
CC = clang
FLAGS = -Wall -Werror -Wextra

SRC = lib.c
SRCDIR = ./srcs/
SRCS = $(addprefix $(SRCDIR), $(SRC))

OBJ = $(SRC:.c=.o)
OBJDIR = ./objs/
OBJS = $(addprefix $(OBJDIR), $(OBJ))

INC = -I ./includes/
SANITIZE = -fsanitize=address,undefined -ferror-limit=10

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rc $(TARGET) $(OBJS)

$(OBJS): $(SRCS)
	$(CC) $(FLAGS) $(INC) $(SANITIZE) -c $(SRCS)
	mv $(OBJ) $(OBJDIR)

clean:
	-rm -rf $(OBJS)
	-rm -rf $(OBJ)

fclean: clean
	-rm $(TARGET)

re:  fclean all
