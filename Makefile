CC = g++
LD = G++

all:
	g++ ec.cpp ec_ops.cpp uberzahl.cpp -o elliptic

debug:
	g++ ec.cpp ec_ops.cpp uberzahl.cpp -g -o elliptic
	
clean:
	rm -f *.o *~ *.out
