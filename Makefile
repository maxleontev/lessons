#all:
#	g++ -fopenmp main.cpp -std=c++11 -o m -lrt -pthread -ltbb
all:
	mpic++ -o m main.cpp
