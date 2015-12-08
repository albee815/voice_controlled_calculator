OBJS = main.o
CC = g++-4.8
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

UtoE : $(OBJS)
	$(CC) -std=c++11 $(LFLAGS) $(OBJS) -o UtoE

main.o : Calculator.h define.h Tokenizer.h Translator.h
	$(CC) -std=c++11 $(CFLAGS) main.cpp

clean:
	\rm *.o *~ UtoE
