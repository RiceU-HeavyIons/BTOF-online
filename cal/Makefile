
G77 = gfortran
CXX = g++
CC = gcc
LD = g++ 
ROOTDEV = /star/u/geurts/lit

INCLUDES      = -I. -I$(ROOTSYS)/include -I$(ROOTDEV)/include #-I$(STAR)/StRoot
ROOTCFLAGS   := $(shell root-config --cflags) 
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --new --glibs)
ROOTGLIBS    += -lNew -lGui
CERNLIBS      = $(shell cernlib) 

#LIBS          = $(ROOTLIBS) $(CERNLIBS) $(SYSLIBS) ./spline/libSplineFit.so
LIBS          = $(ROOTLIBS) -lGui -lHtml -lGuiHtml $(CERNLIBS) $(SYSLIBS) \
	/star/u/geurts/lit/lib/libSplineFit.so \
	/star/u/geurts/lit/lib/libTwoPad.so

GLIBS         = $(ROOTGLIBS) $(SYSLIBS) 

CFLAGS   = -g -O  
CXXFLAGS = ${CFLAGS} -m32 -fno-inline
LDFLAGS  = -m32 -g

#COMP = cal_chain.o 
#BASIC= cal_chain.h 


all: cal

#INPUT = daqdoer
INPUT = muapp

#----------------------------------------------------------------------------------
cal: cal_main.o cal_utils.o cal_loop.o cal_setup.o
	${CXX} cal_main.o cal_utils.o cal_loop.o cal_setup.o \
		$(LDFLAGS) $(LDLIBS) $(LIBS) -o cal

cal_main.o: cal_main.cxx
	${CXX} -c $(CXXFLAGS) -D$(INPUT) $< $(INCLUDES)
cal_utils.o: cal_utils.cxx
	${CXX} -c $(CXXFLAGS) -D$(INPUT) $< $(INCLUDES)
cal_loop.o: cal_loop.cxx
	${CXX} -c $(CXXFLAGS) -D$(INPUT) $< $(INCLUDES)
cal_setup.o: cal_setup.cxx
	${CXX} -c $(CXXFLAGS) -D$(INPUT) $< $(INCLUDES)

cal_main.o: cal_class.h
cal_utils.o: cal_class.h
cal_loop.o: cal_class.h
cal_setup.o: cal_class.h

clean:
	rm cal *.o


