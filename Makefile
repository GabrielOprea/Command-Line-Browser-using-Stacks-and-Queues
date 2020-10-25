EXE=tema2
FLAGS=-g -Wall -Wextra -std=c99
input=_test/input/ex1.in
output=ex1.out
build: $(EXE)
run: $(EXE)
	./$(EXE) $(input) $(output)
$(EXE): TQueue.o tema2.o utils.o TStack.o downloads.o lists.o navigation.o
	gcc TStack.o TQueue.o tema2.o utils.o downloads.o lists.o navigation.o -o $(EXE)
TQueue.o: TQueue.c
	gcc $(FLAGS) -c TQueue.c
TStiva.o: TStiva.c
	gcc $(FLAGS) -c TStack.c
lists.o: lists.c
	gcc $(FLAGS) -c lists.c
downloads.o: downloads.c
	gcc $(FLAGS) -c downloads.c
navigation.o: navigation.c
	gcc $(FLAGS) -c navigation.c
utils.o: utils.c
	gcc $(FLAGS) -c utils.c
tema2.o: tema2.c
	gcc $(FLAGS) -c tema2.c
clean:
	rm $(EXE) *.o

