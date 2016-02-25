
G77 = gfortran
CXX = g++
CC = gcc
LD = g++ 
LDFLAGS  = -L./ -L$(ROOTSYS)/lib
ROOTDEV = /star/u/geurts/lit

INCLUDES      = -I. -I$(ROOTSYS)/include -I$(ROOTDEV)/include #-I$(STAR)/StRoot
ROOTCFLAGS   := $(shell root-config --cflags) 
ROOTLIBS     := $(shell root-config --libs)
ROOTGLIBS    := $(shell root-config --new --glibs)
ROOTGLIBS    += -lNew -lGui
#CERNLIBS      = -L$(CERN)/pro/lib -lkernlib -lpacklib -lmathlib
CERNLIBS      = $(shell cernlib)

#LIBS          = $(ROOTLIBS) $(CERNLIBS) $(SYSLIBS) ./spline/libSplineFit.so
#LIBS          = $(ROOTLIBS) -lGui -lHtml -lGuiHtml $(CERNLIBS) $(SYSLIBS) \
#			/star/u/llope/lit/lib/libSplineFit.so \
#			/star/u/llope/lit/lib/libTwoPad.so
LIBS          = $(ROOTLIBS) -lGui -lHtml -lGuiHtml $(CERNLIBS) $(SYSLIBS) 

GLIBS         = $(ROOTGLIBS) $(SYSLIBS) 

CFLAGS = -g -O  -I. -ITPC
CXXFLAGS = ${CFLAGS} $(INCLUDES)

DBG = -g -m32

all: online readout

#----------------------------------------------------------------------------------
online: online_main.o online_utils.o online_loop.o 
	${CXX} -m32 online_main.o online_utils.o online_loop.o \
		$(INCLUDES) $(LDLIBS) $(LIBS) -o online

readout: readout_main.o readout_utils.o readout_loop.o 
	${CXX} -m32 readout_main.o readout_utils.o readout_loop.o \
		$(INCLUDES) $(LDLIBS) $(LIBS) -o readout

online_main.o: online_main.cxx
	${CXX} -c $(DBG) $< $(INCLUDES)
online_utils.o: online_utils.cxx
	${CXX} -c $(DBG) $< $(INCLUDES)
online_loop.o: online_loop.cxx
	${CXX} -c $(DBG) $< $(INCLUDES)

readout_main.o: readout_main.cxx
	${CXX} -c $(DBG) $< $(INCLUDES)
readout_utils.o: readout_utils.cxx
	${CXX} -c $(DBG) $< $(INCLUDES)
readout_loop.o: readout_loop.cxx
	${CXX} -c $(DBG) $< $(INCLUDES)

online_main.o: online_class.h
online_utils.o: online_class.h
online_loop.o: online_class.h

readout_main.o: readout_class.h
readout_utils.o: readout_class.h
readout_loop.o: readout_class.h


clean:
	rm *.o online readout