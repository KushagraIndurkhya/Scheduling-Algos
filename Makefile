CC = g++
compile:
		${CC} rms.cpp -o rms.o;
		${CC} edf.cpp -o edf.o;
		./rms.o;
		./edf.o;

clean:
		rm *.o;
