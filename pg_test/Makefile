INCLUDE+=-I/usr/local/include -I/usr/local/include/opencv -I/usr/local/include/opencv2 -I/Users/hmliu/boost_1_53_0

LINKING+=-L/usr/lib -Llib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect

CXX=g++
#CXX=ccache g++

SRC=hmm_test.cpp

EXE=hmm_test

OPT=-O3

all:
	$(CXX) $(OPT) $(INCLUDE) $(LINKING) $(SRC) -o $(EXE)

clean:
	rm -f *.o *.a *.out


