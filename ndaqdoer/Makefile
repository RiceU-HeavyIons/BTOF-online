
MYPROGS      = daqdoer

#STARLIBS     = -L$(STAR_LIB) -lRTS -lStEvent -lSt_base -lStarClassLibrary -lStarRoot \
				-lStFtpcTrackMaker -lStChain -lStFtpcClusterMaker -lStDetectorDbMaker
STARLIBS     = -L$(STAR_LIB) -lRTS
STARCFLAGS   = -I$(STAR)/StRoot/RTS/trg/include -I$(STAR)/StRoot/RTS/include \
				-I$(STAR)/StRoot/RTS/src -I$(STAR)/StRoot -I$(STAR)/StRoot/StEvent -I$(STAR)/StRoot/St_base

ROOTCFLAGS   = $(shell root-config --cflags)
ROOTLIBS     = $(shell root-config --libs)
###ROOTGLIBS    = $(shell root-config --glibs) -lNew -lGui -lTable -lGeom

#ROOTGLIBS    = $(shell root-config --glibs) -lNew -lGui -lGeom
#ROOTGLIBS    = $(shell root-config --glibs) -lGeom
ROOTGLIBS =

CXXFLAGS     = -ggdb -DNEW_DAQ_READER -DRTS_LOG_COLORED -DINSIST_ON_EMC_PSEUDO \
				$(ROOTCFLAGS) $(STARCFLAGS)
LDLIBS       = $(STARLIBS) $(ROOTLIBS) $(ROOTGLIBS)

all:	$(MYPROGS)

clean:
	-rm -f *.o
	-rm -f $(MYPROGS)

#daqdoer: main.o StTriggerData.o StTriggerData2009.o doer_pp2pp.o doer_trg.o doer_tof.o TsPacket.o

#daqdoer: main.o StTriggerData.o StTriggerData2009.o doer_pp2pp.o doer_trg.o doer_tof.o doer_mtd.o doer_utils.o TsPacket.o
#daqdoer: main.o tof_err_check.o StTriggerData.o StTriggerData2012.o doer_pp2pp.o doer_trg.o doer_tof.o doer_mtd.o doer_utils.o TsPacket.o
#	$(CXX) -g -m32 -o $@ $(LDLIBS) -Wl,--whole-archive $^ -Wl,--no-whole-archive
#	-mv -v $@ ..
daqdoer: main.o tof_err_check.o StTriggerData.o StTriggerData2018.o doer_pp2pp.o doer_trg.o doer_tof.o doer_mtd.o doer_utils.o TsPacket.o
	$(CXX) -g -m32 -o $@ $(LDLIBS) -Wl,--whole-archive $^ -Wl,--no-whole-archive

main.o: main.cc
	$(CXX) $(CXXFLAGS) -c $<
StTriggerData.o: StTriggerData.cc
	$(CXX) $(CXXFLAGS) -c $<
StTriggerData2009.o: StTriggerData2009.cc
	$(CXX) $(CXXFLAGS) -c $<
#StTriggerData2012.o: StTriggerData2012.cc
#	$(CXX) $(CXXFLAGS) -c $<
#StTriggerData2013.o: StTriggerData2013.cc
#	$(CXX) $(CXXFLAGS) -c $<
StTriggerData2017.o: StTriggerData2017.cc
	$(CXX) $(CXXFLAGS) -c $<
StTriggerData2018.o: StTriggerData2018.cc
	$(CXX) $(CXXFLAGS) -c $<
doer_pp2pp.o: doer_pp2pp.cc
	$(CXX) $(CXXFLAGS) -c $<
doer_trg.o: doer_trg.cc
	$(CXX) $(CXXFLAGS) -c $<
doer_tof.o: doer_tof.cc
	$(CXX) $(CXXFLAGS) -c $<
tof_err_check.o: tof_err_check.cc
	$(CXX) $(CXXFLAGS) -c $<
doer_mtd.o: doer_mtd.cc
	$(CXX) $(CXXFLAGS) -c $<
doer_utils.o: doer_utils.cc
	$(CXX) $(CXXFLAGS) -c $<
TsPacket.o: TsPacket.cxx
	$(CXX) $(CXXFLAGS) -c $<

#*.o: *.cc
#	$(CXX) $(CXXFLAGS) -c $<
#
#*.o: *.cxx
#	$(CXX) $(CXXFLAGS) -c $<
