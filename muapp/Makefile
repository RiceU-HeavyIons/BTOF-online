
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

all: muapp

muapp: muapp.o muapp_main.o muapp_utils.o 
	${CXX} muapp.o muapp_main.o muapp_utils.o $(LDFLAGS) $(INCLUDES) $(LDLIBS) $(LIBS) -o muapp

%.o:%.cxx
	${CXX} $(CXXFLAGS) -c $<
	
clean:
	-rm *.o

muapp.o: muapp.cxx muapp.h 
muapp_main.o: muapp_main.cxx muapp.h
muapp_utils.o: muapp_utils.cxx muapp.h
#TsPacket.o: TsPacket.cxx TsPacket.h


