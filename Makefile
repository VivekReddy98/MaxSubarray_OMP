all:
	g++ Halloween.cpp -o Halloween.out

paralell:
	g++ -g -DDEBUG Halloween.cpp -fopenmp -o Halloween.out

clean:
	rm *.out
