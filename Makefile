CC = g++
LD = G++

all:
	g++ ec.cpp ec_ops.cpp uberzahl.cpp
	
clean:
	rm -f *.o *~
