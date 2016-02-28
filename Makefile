all: m

m: main.o html_parcer.o
	g++ main.o html_parcer.o -o m -lev

main.o: main.cpp
	g++ -std=c++11 -c main.cpp

html_parcer.o: html_parcer.cpp
	g++ -std=c++11 -c html_parcer.cpp

clean:
	rm -rf *.o m
