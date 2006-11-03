/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <PathFind.h>
#include <DynNet.h>
#include <iostream>
/*
This example demonstrates using "PathFind" algorithm.

As side effects, it demonstrates:
1- How to have empty nodes and arcs
2- A simple example with a DynNet.  A node is removed, and new paths are found.  
This can't be done safely with a StaticNet.
*/
typedef struct {} EmptyNode;
typedef struct {} EmptyArc;

std::ostream &operator<<(std::ostream &os, const EmptyNode &en)
{
    return os<<"NODE_DATA_EMPTY";
}
std::ostream &operator<<(std::ostream &os, const EmptyArc &en)
{
    return os<<"ARC_DATA_EMPTY";
}

typedef EmptyNode NodeType;
typedef EmptyArc ArcType;
typedef Cgc::DynNet<NodeType,ArcType> MyNetType;
typedef Cgc::PathFind<MyNetType> PathFinder;

MyNetType *constructNet()
{
    EmptyNode en;
    EmptyArc  ea;
    MyNetType *pNet = new MyNetType(5,7);
    MyNetType::iterator nodeZero = pNet->insert(en);
    MyNetType::iterator nodeOne  = pNet->insert(en);
    MyNetType::iterator nodeTwo  = pNet->insert(en);
    MyNetType::iterator nodeThree= pNet->insert(en);
    MyNetType::iterator nodeFour = pNet->insert(en);
    MyNetType::arc_iterator arcZero = pNet->arc_insert(nodeZero,ea,nodeOne);
    MyNetType::arc_iterator arcOne  = pNet->arc_insert(nodeZero,ea,nodeTwo);
    MyNetType::arc_iterator arcTwo  = pNet->arc_insert(nodeTwo,ea,nodeThree);
    MyNetType::arc_iterator arcThree= pNet->arc_insert(nodeTwo,ea,nodeFour);
    MyNetType::arc_iterator arcFour = pNet->arc_insert(nodeFour,ea,nodeThree);
    MyNetType::arc_iterator arcFive = pNet->arc_insert(nodeZero,ea,nodeThree);
    MyNetType::arc_iterator arcSix  = pNet->arc_insert(nodeThree,ea,nodeTwo);
    std::cout<<"Net:"<<*pNet<<std::endl;
    return pNet;
}

int main()
{
    MyNetType *net = constructNet();
    PathFinder pf(*net,net->find(Cgc::NodeId(0)),net->find(Cgc::NodeId(4))); 
    Cgc::JustPaths paths;
    bool ok = pf.getPaths(paths);
    if(ok)
    {
        std::cout<<"Found "<<paths.size()<<" paths."<<std::endl;
        std::cout<<" Here's a print of all of them."<<std::endl;
        std::cout<<paths<<std::endl;
        std::cout<<" Here's how to get one path out and print it "<<paths[0]<<std::endl;
        Cgc::JustAPath &singlePath = paths[0];
        std::cout<<" Now printing from the reference "<<singlePath<<std::endl;
        std::cout<<" This path has "<<singlePath.size()<<" nodes EXCLUDING THE ROOT"<<std::endl;
        std::cout<<" The first node after the source is "<<singlePath[0]<<std::endl;
        std::cout<<"SUCCESS"<<std::endl;
        net->erase(Cgc::NodeId(3));
        std::cout<<"Here's the reduced network:"<<std::endl;
        std::cout<<*net<<std::endl;
        PathFinder pf2(*net,net->find(Cgc::NodeId(0)),net->find(Cgc::NodeId(4)));
        pf2.getPaths(paths);
        std::cout<<" Now that NodeId3 is gone, we should only have one path:"<<paths.size()<<std::endl;
        std::cout<<paths<<std::endl;
    }
    else
    {
        std::cout<<"FAILED!"<<std::endl;
    }
    return 0;
}
 

