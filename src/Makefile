clean : StereoMatching
		rm -rf *.o

StereoMatching : StereoMatching.o main.o
					g++ -Wall main.o StereoMatching.o -o StereoMatching `pkg-config --cflags --libs opencv`


StereoMatching.o : StereoMatching.cpp StereoMatching.h
					g++ -Wall -c StereoMatching.cpp `pkg-config --cflags --libs opencv`

main.o : main.cpp StereoMatching.h
			g++ -Wall -c main.cpp `pkg-config --cflags --libs opencv`

