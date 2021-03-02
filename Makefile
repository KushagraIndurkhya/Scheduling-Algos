CC = g++
compile:
		${CC} rms.cpp -o rms.o;
		${CC} edf.cpp -o edf.o;
		./rms.o;
		./edf.o

clean:
		rm *.o EDF-Log.txt EDF-Stats.txt RM-Log.txt RM-Stats.txt;
