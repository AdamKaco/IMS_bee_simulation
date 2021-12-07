CC = g++
CFLAGS = -Wall -Wextra -pedantic
NAME = IMS_bee_simulation
all:
	$(CC) $(CFLAGS) $(NAME).cpp -o $(NAME)
	
run:
	./$(NAME)
pack:
	zip 09_xvacla31_xkacoa00 $(NAME).cpp Makefile
clean:
	rm -f *.o *.out $(NAME)
#pack