#	SAPEKTest.cc 

CXXFLAGS=-I../src

.cc.o:
	@echo Compiling $<
	$(CXX) -c $(CXXFLAGS) -o $@ $<

OBJS=	BFTraversalTest.o \
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
	SPStaticTest.o \
	SPTest.o \
	SPTestData.o \
	SPTestDataTest.o \
	SSPSolverTest.o \
	StaticNetTest.o \
	TestData.o \
	TestItem.o \
	TestBed.o \
	SPStaticFBTest.o \
	StaticFBNetTest.o \
	SPLabelCorrTest.o \
	TopoSortTest.o \
	unitTest.o


all: unitTest

unitTest: $(OBJS)
	g++ -I ../src $(OBJS) -o unitTest

clean: 
	rm -f $(OBJS) unitTest *~ *.net 2>/dev/null
