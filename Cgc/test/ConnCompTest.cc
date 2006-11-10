/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#include "ConnComp.h"
#include <StaticNet.h>
#include <StaticFBNet.h>
#include <DynNet.h>
#include "TestItem.h"
#include "TestBed.h"
#include "TestData.h"
#include <string>

typedef StaticNet<NodeLabel,ArcCost> MyNet;
typedef DynNet<NodeLabel,ArcCost> MyNet2;
typedef StaticFBNet<NodeLabel,ArcCost> MyNet3;

template <class NetType>
void test1(const std::string &testName)
{
    TestItem *ti=new TestItem(testName.c_str());
    ConnComp<NetType> *pConnCompConstruction = 
        new ConnComp<NetType>;
    delete pConnCompConstruction;
    ti->passItem();
}

void printResult(CompCollection &comps)
{
    std::cout<<"found "<<comps.size()<<" components"<<std::endl;
    unsigned int cnt=0;
    for(CompCollection::const_iterator cci = comps.begin();
        cci != comps.end();cci++)
    {
        std::cout<<"Component "<<cnt<<" : "<<std::endl;
        std::cout<<*cci<<std::endl;
        std::cout<<"---------------------"<<std::endl;
        cnt++;
    }
}


template <class NetType>
void netConstruction(NetType &net)
{
    //
    // three nodes in a cycle.
    //
    NodeLabel zero;
    NodeLabel one;
    NodeLabel two;
    NetType::iterator zeroIter = net.insert(zero);
    std::cout<<"net="<<net<<std::endl;
    NetType::iterator oneIter = net.insert(one);
    std::cout<<"net="<<net<<std::endl;
    NetType::iterator twoIter = net.insert(two);
    std::cout<<"net="<<net<<std::endl;
    ArcCost arcZero;
    std::cout<<"net="<<net<<std::endl;
    net.arc_insert(zeroIter,arcZero,oneIter);
    std::cout<<"net="<<net<<std::endl;
    net.arc_insert(zeroIter,arcZero,twoIter);
    std::cout<<"net="<<net<<std::endl;
    net.arc_insert(oneIter,arcZero,twoIter);
    std::cout<<"net="<<net<<std::endl;
    net.arc_insert(twoIter,arcZero,zeroIter);
    std::cout<<"net="<<net<<std::endl;
}


template <class NetType>
void test2(const std::string &testName)
{
    TestItem *ti=new TestItem(testName.c_str());
    NetType net(3,5);
    netConstruction<NetType>(net);

    ConnComp<NetType> connComp;

    CompCollection comps;

    bool bResult = connComp.solve( comps,
        net,
        net.begin(),
        net.end());
    if(bResult!=true)
        ti->failItem(__SPOT__);
    if(comps.size()!=2)
        ti->failItem(__SPOT__);
    comps.mergeComponents();
    if(comps.size()!=1)
    {
        std::cout<<"Collection at failure:"<<comps<<std::endl;
        ti->failItem(__SPOT__);
    }
    //printResult(comps);
    ti->passItem();
}

template <class NetType>
void netConstruction2(NetType &net)
{
    //
    // three nodes in a cycle.
    //
    NodeLabel zero;
    NodeLabel one;
    NodeLabel two;
    NetType::iterator zeroIter = net.insert(zero);
    NetType::iterator oneIter = net.insert(one);
    NetType::iterator twoIter = net.insert(two);
    NetType::iterator threeIter = net.insert(two);
    ArcCost arcZero;
    net.arc_insert(zeroIter,arcZero,oneIter);
    net.arc_insert(zeroIter,arcZero,twoIter);
    net.arc_insert(oneIter,arcZero,twoIter);
    net.arc_insert(oneIter,arcZero,threeIter);
    net.arc_insert(twoIter,arcZero,zeroIter);
    net.arc_insert(twoIter,arcZero,threeIter);
}


template <class NetType>
void test3(const std::string &testName)
{
    TestItem *ti=new TestItem(testName.c_str());
    NetType net(4,6);
    netConstruction2(net);
    ConnComp<NetType> connComp;

    CompCollection comps;
    bool bResult = false;
    bResult = connComp.solve( comps,
        net,
        net.begin(),
        net.end());

    if(bResult!=true)
        ti->failItem(__SPOT__);
    if(comps.size()!=2)
        ti->failItem(__SPOT__);
    comps.mergeComponents();
    if(comps.size()!=1)
    {
        std::cout<<"Components in failure="<<comps<<std::endl;
        ti->failItem(__SPOT__);
    }
    ti->passItem();
}


int ConnCompTest(TestBed &myBed)
{
    TestItem::setBed(&myBed);

//    test1<MyNet>("ConnComp:Simple Construction:StaticNet");
//    test1<MyNet2>("ConnComp:Simple Construction:DynNet");
    test1<MyNet3>("ConnComp:Simple Construction:StaticFBNet");
//    test2<MyNet>("ConnComp:Simple Network solve:StaticNet");
//    test2<MyNet2>("ConnComp:Simple Network solve:DynNet");
    test2<MyNet3>("ConnComp:Simple Network solve:StaticFBNet");
//    test3<MyNet>("ConnComp:Bigger net, same answer.:StaticNet");
//    test3<MyNet2>("ConnComp:Bigger net, same answer.:DynNet");
    test3<MyNet3>("ConnComp:Bigger net, same answer.:StaticFBNet");
    return 0;
}
