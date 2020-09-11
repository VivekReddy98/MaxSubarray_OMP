serial:
	g++ Halloween.cpp -o Halloween.out

paralell:
	g++ Halloween.cpp -fopenmp -DPRL -o Halloween.out

clean:
	rm *.out
