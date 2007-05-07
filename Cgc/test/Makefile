#	SAPEKTest.cc 

CXXFLAGS=-I../src

.cc.o:
	@echo Compiling $<
	$(CXX) -c $(CXXFLAGS) -o $@ $<

OBJS= BFTraversalTest.o \
	ConnCompTest.o  \
	DFTraversalTest.o \
	DynNetTest.o \
	DIMACSFormatTest.o \
	DotOutputFormatTest.o \
	LayerSPTest.o \
	MSTData.o \
	MSTDataTest.o \
	MSTTest.o \
	NodeIdTest.o \
	PathFindTest.o \
	PathTest.o \
	SPDynTest.o \
	SPStaticFBTest.o \
	SPStaticTest.o \
	SPTest.o \
	SPTestData.o \
	SPTestDataTest.o \
	SSPSolverTest.o \
	StaticFBNetTest.o \
	StaticNetTest.o \
	TestData.o \
	TestItem.o \
	TestBed.o \
	SPLabelCorrTest.o \
	unitTest.o

all: unitTest

unitTest: $(OBJS)
	g++ -I ../src $(OBJS) -o unitTest

clean: 
	rm -f $(OBJS) unitTest *~ *.net 2>/dev/null
