
G77 = gfortran
CXX = g++
CC = gcc
LD = g++
LDFLAGS  = -L./ -L$(ROOTSYS)/lib

INCLUDES      = -I. -I$(ROOTSYS)/include
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --new --glibs)

LIBS          = $(ROOTLIBS) -lGui -lHtml -lGuiHtml

CFLAGS =  -g -O  -I. -ITPC
CXXFLAGS = ${CFLAGS} ${ROOTCFLAGS}
DBG = -g

all: noise

clean:
	-rm -f *.o
	-rm -f noise
#----------------------------------------------------------------------------------
noise: noise_main.o noise_utils.o noise_loop.o
	${CXX} ${CXXFLAGS} noise_main.o noise_utils.o noise_loop.o \
		 $(INCLUDES) $(LDLIBS) $(LIBS) -o noise

noise_main.o: noise_main.cxx
	${CXX} ${CXXFLAGS} -c $(DBG) $< $(INCLUDES)
noise_utils.o: noise_utils.cxx
	${CXX} ${CXXFLAGS} -c $(DBG) $< $(INCLUDES)
noise_loop.o: noise_loop.cxx
	${CXX} ${CXXFLAGS} -c $(DBG) $< $(INCLUDES)

noise_main.o: noise_class.h
noise_utils.o: noise_class.h
noise_loop.o: noise_class.h
