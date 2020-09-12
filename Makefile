all:
	g++ MaxCandy.cpp -o MaxCandy.out

paralell:
	g++ MaxCandy.cpp -fopenmp -o MaxCandy.out

clean:
	rm *.out
