/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/

/* The USESTATIC demonstrates that the basic graph iterating occurs
   on more than one graph type (StaticNet or DynNet).
   For this example, we will choose USESTATIC, but feel free to test by
   commenting out the #define line.

   I'm using the typedef for 2 reasons:
   - Easy of reference to the graph type
   - Facilitating writing the code once even though I might swap out the
     graph type later.
   
*/
#define USESTATIC
#ifdef USESTATIC

#include <StaticNet.h>
using namespace Cgc;
typedef StaticNet<int,int> MyNet;

#else

#include <DynNet.h>
typedef DynNet<int,int> MyNet;

#endif


int main()
{
    // declare the graph with sizes.
    MyNet net(3,3);
    // Insert 3 nodes.  (Note, these must be in order for the static net)
    std::cout<<"Inserting 3 nodes"<<std::endl;
    MyNet::iterator nodeIt0 = net.insert(1);
    MyNet::iterator nodeIt1 = net.insert(2);
    MyNet::iterator nodeIt2 = net.insert(3);
    // Demonstrate getting node Ids for the inserted nodes.
    std::cout<<" inserted nodes:"
             <<net.getNodeId(nodeIt0)<<" "
             <<net.getNodeId(nodeIt1)<<" "
             <<net.getNodeId(nodeIt2)<<std::endl;
    // Insert the arcs.
    // NOTE: for the static network, these must be inserted in
    // tail-head sorted order.  Any other order would cause inefficiencies
    // in costruction and is not allowed.  It will cause a runtime failure.
    MyNet::arc_iterator arcIt = net.arc_insert(nodeIt0,1,nodeIt1);
    net.arc_insert(nodeIt0,2,nodeIt2);
    net.arc_insert(nodeIt1,3,nodeIt2);
    std::cout<<"Printing the network we just built"<<std::endl;
    std::cout<<net<<std::endl;
    return 0;
}
