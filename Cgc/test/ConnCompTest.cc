/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#include "ConnComp.h"
#include <StaticNet.h>
#include "TestItem.h"
#include "TestBed.h"
#include "TestData.h"

typedef StaticNet<NodeLabel,ArcCost> MyNet;
typedef ConnComp<MyNet> MyConnComp;

void test1()
{
    TestItem *ti=new TestItem("ConnComp:Simple Construction");
    ConnComp<MyNet> *pConnCompConstruction = 
        new ConnComp<MyNet>;
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


void netConstruction(MyNet &net)
{
    //
    // three nodes in a cycle.
    //
    NodeLabel zero;
    NodeLabel one;
    NodeLabel two;
    MyNet::iterator zeroIter = net.insert(zero);
    MyNet::iterator oneIter = net.insert(one);
    MyNet::iterator twoIter = net.insert(two);
    ArcCost arcZero;
    net.arc_insert(zeroIter,arcZero,oneIter);
    net.arc_insert(zeroIter,arcZero,twoIter);
    net.arc_insert(oneIter,arcZero,twoIter);
    net.arc_insert(twoIter,arcZero,zeroIter);

}


void test2()
{
    TestItem *ti=new TestItem("ConnComp:Simple Network solve");
    MyNet net(3,5);
    netConstruction(net);

    MyConnComp connComp;;

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
void netConstruction2(MyNet &net)
{
    //
    // three nodes in a cycle.
    //
    NodeLabel zero;
    NodeLabel one;
    NodeLabel two;
    MyNet::iterator zeroIter = net.insert(zero);
    MyNet::iterator oneIter = net.insert(one);
    MyNet::iterator twoIter = net.insert(two);
    MyNet::iterator threeIter = net.insert(two);
    ArcCost arcZero;
    net.arc_insert(zeroIter,arcZero,oneIter);
    net.arc_insert(zeroIter,arcZero,twoIter);
    net.arc_insert(oneIter,arcZero,twoIter);
    net.arc_insert(oneIter,arcZero,threeIter);
    net.arc_insert(twoIter,arcZero,zeroIter);
    net.arc_insert(twoIter,arcZero,threeIter);
}


void test3()
{
    TestItem *ti=new TestItem("ConnComp:Bigger net, samne answer.");
    MyNet net(4,6);
    netConstruction2(net);

    MyConnComp connComp;;

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
        std::cout<<"Components in failure="<<comps<<std::endl;
        ti->failItem(__SPOT__);
    }
    ti->passItem();
}


int ConnCompTest(TestBed &myBed)
{
    TestItem::setBed(&myBed);

    std::cout<<"Testing StaticNet with ConnComp"<<std::endl;
    test1();
    test2();
    test3();
    std::cout<<"\n----<All tests complete>----\n"<<std::endl;
    return 0;
}
