CXXFLAGS=-I../src

.cc.o:
	@echo Compiling $<
	$(CXX) -c $(CXXFLAGS) -o $@ $<

OBJS= sspsolver.o 
SIMPLEOBJS= simple.o 
SIMPLESHORTESTPATHOBJS= simpleShortestPath.o 
PATHFINDEXAMPLEOBJS= pathFindExample.o 

all: sspsolver simple simpleShortestPath pathFindExample

sspsolver: $(OBJS)
	g++ -I ../src $(OBJS) -o sspsolver

simpleShortestPath: $(SIMPLESHORTESTPATHOBJS)
	g++ -I ../src $(SIMPLESHORTESTPATHOBJS) -o simpleShortestPath

simple: $(SIMPLEOBJS)
	g++ -I ../src $(SIMPLEOBJS) -o simple

pathFindExample: $(PATHFINDEXAMPLEOBJS)
	g++ -I ../src $(PATHFINDEXAMPLEOBJS) -o pathFindExample


clean: 
	rm -f $(OBJS) sspsolver $(SIMPLEOBJS) simple $(SIMPLESHORTESTPATHOBJS) simpleShortestPath $(PATHFINDEXAMPLEOBJS) pathFindExample
