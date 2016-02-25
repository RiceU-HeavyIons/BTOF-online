
G77 = gfortran
CXX = g++
CC = gcc
LD = g++ 
LDFLAGS  = -L./ -L$(ROOTSYS)/lib

INCLUDES      = -I. -I$(ROOTSYS)/include 
#-I$(ROOTDEV)/include #-I$(STAR)/StRoot
ROOTCFLAGS   := $(shell root-config --cflags) 
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --new --glibs)
ROOTGLIBS    += -lNew -lGui
CERNLIBS      = -L$(CERN)/pro/lib -lkernlib -lpacklib -lmathlib

#LIBS          = $(ROOTLIBS) $(CERNLIBS) $(SYSLIBS) ./spline/libSplineFit.so
#LIBS          = $(ROOTLIBS) -lGui -lHtml -lGuiHtml $(CERNLIBS) $(SYSLIBS) \
#			/star/u/llope/lit/lib/libSplineFit.so \
#			/star/u/llope/lit/lib/libTwoPad.so
LIBS          = $(ROOTLIBS) -lGui -lHtml -lGuiHtml $(CERNLIBS) $(SYSLIBS) 

GLIBS         = $(ROOTGLIBS) $(SYSLIBS) 

CFLAGS = -g -O  -I. -ITPC
CXXFLAGS = ${CFLAGS} $(INCLUDES)
DBG = -g -m32

all: noise

#----------------------------------------------------------------------------------
noise: noise_main.o noise_utils.o noise_loop.o 
	${CXX} noise_main.o noise_utils.o noise_loop.o \
		-m32 $(INCLUDES) $(LDLIBS) $(LIBS) -o noise

noise_main.o: noise_main.cxx
	${CXX} -c $(DBG) $< $(INCLUDES)
noise_utils.o: noise_utils.cxx
	${CXX} -c $(DBG) $< $(INCLUDES)
noise_loop.o: noise_loop.cxx
	${CXX} -c $(DBG) $< $(INCLUDES)

noise_main.o: noise_class.h
noise_utils.o: noise_class.h
noise_loop.o: noise_class.h




