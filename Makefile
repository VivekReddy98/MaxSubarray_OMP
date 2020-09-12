all:
	g++ Halloween.cpp -o Halloween.out

paralell:
	g++ Halloween.cpp -fopenmp -o Halloween.out

clean:
	rm *.out
