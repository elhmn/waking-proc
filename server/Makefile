TARGET = waking-proc-server
CC = clang
FLAGS = -Wall -Werror -Wextra

SRC = main.c parse_options.c
SRCDIR = ./srcs/
SRCS = $(addprefix $(SRCDIR), $(SRC))

OBJ = $(SRC:.c=.o)
OBJDIR = ./objs/
OBJS = $(addprefix $(OBJDIR), $(OBJ))

WAKING_PROC_LIB_DIR=../lib
WAKING_PROC_LIB=$(WAKING_PROC_LIB_DIR)/libwaking-proc.a
INC = -I ./includes/ -I $(WAKING_PROC_LIB_DIR)/includes/
SANITIZE = -fsanitize=address,undefined

all: $(TARGET)

.PHONY: waking-proc-lib
waking-proc-lib: $(WAKING_PROC_LIB)

$(WAKING_PROC_LIB):
	$(MAKE) -C $(WAKING_PROC_LIB_DIR)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) -o $(TARGET) $(SANITIZE) $(OBJS) -L $(WAKING_PROC_LIB_DIR) -lwaking-proc

$(OBJS): $(WAKING_PROC_LIB)
	$(CC) $(FLAGS) $(INC) -c $(SRCS)
	mv $(OBJ) $(OBJDIR)


clean:
	-rm -rf $(OBJS)
	-rm -rf $(OBJ)

fclean: clean
	-rm -rf $(WAKING_PROC_LIB)
	-rm $(TARGET)

re:  fclean waking-proc-lib all
