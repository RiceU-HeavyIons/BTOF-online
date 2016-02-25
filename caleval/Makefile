
G77 = gfortran
CXX = g++
CC = gcc
LD = g++ 
ROOTDEV = /star/u/geurts/lit

INCLUDES      = -I. -I$(ROOTSYS)/include #-I$(ROOTDEV)/include #-I$(STAR)/StRoot
ROOTCFLAGS   := $(shell root-config --cflags) 
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --new --glibs)
ROOTGLIBS    += -lNew -lGui
CERNLIBS      = $(shell cernlib) 

#LIBS          = $(ROOTLIBS) $(CERNLIBS) $(SYSLIBS) ./spline/libSplineFit.so
LIBS          = $(ROOTLIBS) -lGui -lHtml -lGuiHtml $(CERNLIBS) $(SYSLIBS) 
#	/star/u/geurts/lit/lib/libSplineFit.so \
#	/star/u/geurts/lit/lib/libTwoPad.so

GLIBS         = $(ROOTGLIBS) $(SYSLIBS) 

CFLAGS   = -g -O -m32
CXXFLAGS = -g ${CFLAGS} -fno-inline -m32
LDFLAGS  = -m32

#COMP = caleval_chain.o 
#BASIC= caleval_chain.h 

all: caleval

#INPUT = daqdoer
INPUT = muapp

#----------------------------------------------------------------------------------
caleval: caleval_main.o caleval_utils.o caleval_loop.o caleval_setup.o
	${CXX} caleval_main.o caleval_utils.o caleval_loop.o caleval_setup.o \
		$(LDFLAGS) $(LDLIBS) $(LIBS) -o caleval

caleval_main.o: caleval_main.cxx
	${CXX} -c $(CXXFLAGS) -D$(INPUT) $< $(INCLUDES)
caleval_utils.o: caleval_utils.cxx
	${CXX} -c $(CXXFLAGS) -D$(INPUT) $< $(INCLUDES)
caleval_loop.o: caleval_loop.cxx
	${CXX} -c $(CXXFLAGS) -D$(INPUT) $< $(INCLUDES)
caleval_setup.o: caleval_setup.cxx
	${CXX} -c $(CXXFLAGS) -D$(INPUT) $< $(INCLUDES)

caleval_main.o: caleval_class.h
caleval_utils.o: caleval_class.h
caleval_loop.o: caleval_class.h
caleval_setup.o: caleval_class.h

clean:
	rm caleval *.o




