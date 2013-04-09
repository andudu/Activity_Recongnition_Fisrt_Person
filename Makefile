#Macbook

#INCLUDE+=-I/usr/local/include -I/usr/local/include/opencv -I/usr/local/include/opencv2

#LINKING+=-L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_objdetect

##################################################################################################

#Ubuntu

INCLUDE+=-I/usr/local/include

LINKING+=-L/usr/local/lib

CV_CONF=`pkg-config opencv --cflags`
CV_CONF_2=`pkg-config opencv --libs`

#################################################################################################

CXX=g++
#CXX=ccache g++

SRC=main.cpp main.h frames.cpp frames.h haar_cascade.cpp haar_cascade.h objectDetector.cpp objectDetector.h temporalPyramid.cpp temporalPyramid.h activityDetector.cpp activityDetector.h

EXE=FP_ADL.out

OPT=-O0

all:
	$(CXX) $(CV_CONF) $(OPT) $(INCLUDE) $(LINKING) $(SRC) -o $(EXE) $(CV_CONF_2)

clean:
	rm -f *.o *.a *.out


