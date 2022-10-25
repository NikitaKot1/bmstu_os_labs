CC := gcc

CFLAGS := -std=c99 -Wall -Werror -I inc/ -g3

OBJS := actions.o examination.o time.o

.PHONY : clean

app: $(OBJS) main.o
	$(CC) $^ -o $@

%.o : src/%.c inc/*.h
	$(CC) $(CFLAGS) -c $<

clean :
	rm *.o
