
G77 = g77
CXX = g++
CC = gcc
LD = g++ 

INCLUDES      = -I. -I$(ROOTSYS)/include #-I$(STAR)/StRoot
ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --new --glibs)
ROOTGLIBS    += -lNew -lGui
#CERNLIBS      = -L$(CERN)/pro/lib -lkernlib -lpacklib -lmathlib
CERNLIBS      = $(shell cernlib)
LIBS          = $(ROOTLIBS) $(CERNLIBS) $(SYSLIBS)
GLIBS         = $(ROOTGLIBS) $(SYSLIBS)

CFLAGS = -O3  -m32 -fno-inline
CXXFLAGS = -g ${CFLAGS} $(INCLUDES)
#LDFLAGS  = -L./ -L$(ROOTSYS)/lib
LDFLAGS  = -m32

all: dataqa

dataqa: dataqa_loop.o dataqa_main.o dataqa_utils.o 
	${CXX} dataqa_loop.o dataqa_main.o dataqa_utils.o $(LDFLAGS) $(INCLUDES) $(LDLIBS) $(LIBS) -o dataqa

%.o:%.cxx
	${CXX} $(CXXFLAGS) -c $<
	
clean:
	-rm *.o

dataqa.o: dataqa.cxx dataqa_class.h 
dataqa_main.o: dataqa_main.cxx dataqa_class.h
dataqa_utils.o: dataqa_utils.cxx dataqa_class.h


