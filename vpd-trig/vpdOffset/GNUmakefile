OBJS = trgEvent.o analysis.o
EXE = analysis

OBJSREL = trgEvent.o analysis_relative.o
EXEREL = analysis_relative

#OBJS = MuDst.o createphiwgt.o
#EXE = createphiwgt

#OBJS = MuDst.o StPhysicalHelixD.o StHelixD.o StTofGeometry.o createphiwgt.o
#EXE = createphiwgt

ROOTCFLAGS    = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --libs)
ROOTGLIBS     = $(shell root-config --glibs)

INCFLAGS = -I$(ROOTSYS)/include
LDFLAGS = -L$(ROOTSYS)/lib
LDLIBS_PREFINAL = -lstdc++
 
LDLIBS_PREFINAL +=   -m32 \
                     -fno-inline \
                     -lGpad \
                     -lCore \
                     -lCint \
                     -lHist \
                     -lGraf \
                     -lGraf3d \
                     -lTree \
                     -lRint \
                     -lRIO \
                     -lNet \
                     -lPostscript \
                     -lMatrix \
                     -lPhysics \
                     -ldl \
                     -lm \
                     -rdynamic

CXX = g++
FLAGS = -Wall -g $(INCFLAGS) $(LDFLAGS) -m32 -fno-inline

COMPILE = $(CXX) $(FLAGS) -c 

all: $(EXE) $(EXEREL)

$(EXE): $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(LDLIBS_PREFINAL) $(ROOTFLAGS) $(ROOTLIBS)

$(EXEREL): $(OBJSREL)
	$(CXX) -o $(EXEREL) $(OBJSREL) $(LDLIBS_PREFINAL) $(ROOTFLAGS) $(ROOTLIBS)

%.o: %.cxx
	$(COMPILE)  $< 

# analysis_relative.o: analysis_relative.cxx 
# 	$(COMPILE)  $< 