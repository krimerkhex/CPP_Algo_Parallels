FLAGS = -Wall -Werror -Wextra -std=c++17 -pthread
FILES = Parallels.cc s21_console_menu.cc ant.a gauss.a winograd.a

all: install run

run:
	./Parallels.out

install: ant gauss winograd
	g++ $(FLAGS) $(FILES) -o Parallels.out

ant:
	g++ $(FLAGS) -c s21_graph.cc s21_graph_algorithms.cc
	ar rcs ant.a s21_graph.o s21_graph_algorithms.o
	ranlib ant.a

gauss:
	g++ $(FLAGS) -c s21_slau_algoritms.cc
	ar rcs gauss.a s21_slau_algoritms.o
	ranlib gauss.a

winograd:	
	g++ $(FLAGS) -c s21_mul_algorithms.cc
	ar rcs winograd.a s21_mul_algorithms.o
	ranlib winograd.a

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf *.out
