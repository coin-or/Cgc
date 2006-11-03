/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
/*
  In this sample, we're constructing a static network, then solving a
  single source to all sinks shortest path problem on the graph.

  The Static network graph type is in StaticNet.h
  -- vector based storage for the graph, size known up front.
  The Dijkstra-based algorithm is in DijkSolver.h
  -- requires getLabel and setLabel on the node, and getCost on the arc type.
  -- requires 'cost' to be integer.  Minor mod (extra template parm) to make
     the data type more arbitrary than "int"
 */

#include <StaticNet.h>
#include <DijkSolver.h>
#include <assert.h>
using namespace Cgc;
/* For DijkSolver, the node type is required to have these methods.
   It will fail at compile time if it does not.
   Note: Costs are always ints for this solver.
*/
class NodeType
{
public:
    // required for construction.
    NodeType()
    :cost(0),
     backId(0)
    { }
    // This is required by DijkSolver.  It sets the cost and predecessor.
    void setLabel(int pathCost, NodeId prevNode)
    {
        cost=pathCost;
        backId=prevNode;
    }
    // Returns the current cost of the node. Required by DijkSolver.
    int getLabel()const
    {
        return cost;
    }

    // Returns the current predecessor of the node. Required by DijkSolver.
    NodeId getPred()const
    {
        return backId;
    }
private:
    // Storage for the cost and back pointer "label".
    int cost;
    NodeId backId;
    // Some handy friend and forward declarations.
    friend std::ostream &operator<<(std::ostream &os, const NodeType &nt);
    // This method is required for DijkSolver to compare labels.
    friend bool operator>(const NodeType &node1, const NodeType &node2);
};

// method compares two labels.  This is required for DijkSolver
bool operator>(const NodeType &node1, const NodeType &node2)
{
    return node1.cost > node2.cost;
}

// handy print.  It is used later in the network print operator<<
std::ostream &operator<<(std::ostream &os, const NodeType &nt)
{
    return os<<" Cost="<<nt.cost<<" and prevNode="<<nt.backId;
}

/* Representation of one arc in the grpah.  Must have all these methods
   for DijkSolver.
   Note: Costs are always ints for this solver.
 */
class ArcType
{
public:
    ArcType(int inCost)
    :cost(inCost)
    { }

    ArcType() :cost(0) { }

    /* This method allows one to selectively and non-destructively
       turn on and off arcs in the graph.  If the method returns "false" then
       DijkSolver will ignore that arc.
       This version of DijkSolver requires it.
       This kind of capability is useful if one wishes to frequently
       perturb the graph and re-solve.
       Note: One can simulate a Node on/off by turning off all arcs
       leading into that node.  This is left as an exercise.
    */
    bool valid()const
    {
        return true;
    }
    // returns the cost of the arc.
    int getCost()const
    {
        return cost;
    }
private:
    int cost;
    // useful print used later.  Not strictly required by DijkSolver.
    friend std::ostream &operator<<(std::ostream &os, const ArcType &nt);
};

// print used later.
std::ostream &operator<<(std::ostream &os, const ArcType &nt)
{
    return os<<" arc cost:"<<nt.cost<<' ';
}

// Finally, we can declare our graph.
typedef StaticNet<NodeType,ArcType> MyNet;

// Further, we can declare our solver.
typedef DijkSolver<MyNet,MyNet::iterator> MySolver;

// In "real" code the graph is constructed somehow.  In this case the
// values are hard coded.
void constructNetwork(MyNet &net)
{
    MyNet::iterator nodeIt0 = net.insert(NodeType());
    MyNet::iterator nodeIt1 = net.insert(NodeType());
    MyNet::iterator nodeIt2 = net.insert(NodeType());
    //The only real thing of interest: The path 0-1-2 is cheaper than 0-2.
    MyNet::arc_iterator arcIt = net.arc_insert(nodeIt0,1,nodeIt1);
    net.arc_insert(nodeIt0,4,nodeIt2);
    net.arc_insert(nodeIt1,2,nodeIt2);
}


int main()
{
    // declare the network of correct size.
    MyNet net(3,3);
    
    // call the construction method.
    constructNetwork(net);
    // Print out contents before the solve.
    std::cout<<"PRE_SOLVE"<<net<<std::endl;

    // Declare a solver.
    MySolver solver;
    // Find the source node.
    MyNet::iterator sourceNode = net.find(NodeId(0));
    // Call solve which sets the labels.
    solver.solve(net,sourceNode);

    // Pring out the graph post-solve.
    std::cout<<"POST_SOLVE"<<net<<std::endl;

    // Now look up paths for the other nodes.  These will all be paths
    // back to the source from these other nodes.
    // further, the node IDs will be in the Path in reverse order.
    // This is due to the back-track nature of the lookup.
    MyNet::iterator sink1 = net.find(NodeId(1));
    MyNet::iterator sink2 = net.find(NodeId(2));
    
    // declare some paths to fill up with the result.
    Path path1;
    Path path2;
    
    // Go get the shortest paths from source to sinks.
    solver.getPath(net,sink1,path1);
    solver.getPath(net,sink2,path2);
    
    // Print out the paths.  Note that if one wishes them in
    // "normal" order, use rbegin() and rend() instead of begin and end() as
    // done in this operator<< for printing Path.
    std::cout<<"Shortest path from 0 to 1=>"<<path1<<std::endl;
    std::cout<<"Shortest path from 0 to 2=>"<<path2<<std::endl;

    return 0;
}
