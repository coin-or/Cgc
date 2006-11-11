/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#include <iostream>
#include <fstream>
#include <StaticNet.h>
#include <DynNet.h>
#include <StaticFBNet.h>
#include <GraphDotFormat.h>
#include "TestItem.h"
using namespace Cgc;
typedef StaticNet<int,int> MyNetType;
typedef DynNet<int,int> MyNetType2;
typedef StaticFBNet<int,int> MyNetType3;

template <class NetType>
NetType * constructNet()
{
    NetType *net = new NetType(3,4);
    //
    // three nodes in a cycle.
    //
    typename NetType::iterator zeroIter = net->insert(0);
    typename NetType::iterator oneIter = net->insert(1);
    typename NetType::iterator twoIter = net->insert(2);
    int arcZero=0;
    net->arc_insert(zeroIter,arcZero,oneIter);
    net->arc_insert(zeroIter,arcZero,twoIter);
    net->arc_insert(oneIter,arcZero,twoIter);
    net->arc_insert(twoIter,arcZero,zeroIter);
    return net;
}

template<class NetType>
void testOutput(const std::string &testName, const std::string &fileName)
{
    TestItem *ti = new TestItem(testName.c_str());
    MyNetType *net = constructNet<MyNetType>();
    GraphDotFormat<NetType> formatter("MyGraph",*net);
    formatter.print(std::cout);
    std::ofstream outFile(fileName.c_str());
    formatter.print(outFile);
    ti->passItem();
}

void DotOutputFormatTest(TestBed &bed)
{
    TestItem::setBed(&bed);
    testOutput<MyNetType>("StaticNet Dot format output","testNet.dot");
    testOutput<MyNetType>("DynNet Dot format output","testNet2.dot");
    testOutput<MyNetType>("StaticFBNet Dot format output","testNet3.dot");
}
