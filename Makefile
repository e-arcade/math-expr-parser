CC=gcc -std=c11
CFLAGS=-Wall -Werror -Wextra
SRCS=$(wildcard *.c)
OBJS=$(SRCS:c=o)
LIB_NAME=math-expr-parser

all: clean $(LIB_NAME).a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME).a: $(OBJS)
	ar rc $(LIB_NAME).a $^
	ranlib $(LIB_NAME).a
	rm -rf *.o

clean:
	rm -rf *.o *.a *.out

rebuild: all 