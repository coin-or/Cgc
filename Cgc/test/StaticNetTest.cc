/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <StaticNet.h>
#include "TestData.h"

#include <iostream>
#include "TestItem.h"
#include "TestBed.h"


typedef StaticNet< NodeLabel, ArcCost > MyNetType;
typedef StaticNet< NodeLabel, ArcCost >::Node MyNodeType;
typedef StaticNet< NodeLabel, ArcCost >::Arc MyArcType;

void buildBasicNetPlus(MyNetType &netToFill)
{
    static TestItem *item1 = new TestItem("StaticNetTest:aux:buildBasic");
  
    item1->passItem();
    NodeLabel zero(0);
    //cout<<"aux.1"<<std::endl;
    MyNetType::iterator nodeIt = netToFill.insert(zero);
    if(*(*nodeIt)!=zero)
        item1->failItem(__SPOT__);
  
    NodeLabel one(1);
    nodeIt = netToFill.insert(one);
    if(*(*nodeIt)!=one)
        item1->failItem(__SPOT__);
  
    //cout<<"aux.2"<<std::endl;
    MyNetType::arc_iterator arcIt =
        netToFill.arc_insert(netToFill[0],ArcCost(3),netToFill[1]);
  
    if(arcIt == netToFill.arc_end())
        item1->failItem(__SPOT__);
  
    NodeLabel two(2);
    nodeIt = netToFill.insert(two);
    if(*(*nodeIt)!=two)
        item1->failItem(__SPOT__);
  
    ArcCost arcCost(3);
    arcIt = netToFill.arc_insert(netToFill.find(NodeId(0)),
                                 arcCost,netToFill.find(NodeId(1)));
  
    if(arcIt == netToFill.arc_end())
        item1->failItem(__SPOT__);

    NodeLabel three(0);
    nodeIt = netToFill.insert(three);
  
}

void buildBigNet(MyNetType &netToFill)
{
    static TestItem *item1 = new TestItem("StaticNetTest:aux2:buildBig");
  
    item1->passItem();
    NodeLabel zero(0);
    MyNetType::iterator zeroIt = netToFill.insert(zero);
    if(*(*zeroIt)!=zero)
        item1->failItem(__SPOT__);
  
    NodeLabel one(1);
    MyNetType::iterator oneIt = netToFill.insert(one);
    if(*(*oneIt)!=one)
        item1->failItem(__SPOT__);
  
    MyNetType::arc_iterator arcOne =
        netToFill.arc_insert(zeroIt,
                             ArcCost(3),
                             oneIt);
  
    if(arcOne == netToFill.arc_end())
        item1->failItem(__SPOT__);
    if(*(*arcOne)!=ArcCost(3))
        {
        std::cout<<"\nFAILURE:arcOne="<<*(*arcOne)<<std::endl;
        item1->failItem(__SPOT__);
        }
    if((*arcOne).head()!=oneIt)
        item1->failItem(__SPOT__);
  
    NodeLabel two(2);
    MyNetType::iterator twoIt = netToFill.insert(two);
    if(*(*twoIt)!=two)
        item1->failItem(__SPOT__);
  
    ArcCost arcCost(3);
    MyNetType::arc_iterator arcTwo = 
        netToFill.arc_insert(zeroIt,
                             arcCost,
                             twoIt);
    if(arcTwo == netToFill.arc_end())
        item1->failItem(__SPOT__);
  
    MyNetType::arc_iterator arcThree = 
        netToFill.arc_insert(oneIt,
                             ArcCost(4),
                             twoIt);
    if(arcThree == netToFill.arc_end())
        item1->failItem(__SPOT__);
  
    MyNetType::arc_iterator arcFive = 
        netToFill.arc_insert(oneIt,
                             ArcCost(6),
                             zeroIt);

    if(arcFive == netToFill.arc_end())
        item1->failItem(__SPOT__);

    MyNetType::arc_iterator arcFour = 
        netToFill.arc_insert(twoIt,
                             ArcCost(5),
                             zeroIt);
    if(arcFour == netToFill.arc_end())
        item1->failItem(__SPOT__);
  
  
}

void buildBasicNet(MyNetType &netToFill)
{
    static TestItem *item1 = new TestItem("StaticNetTest:aux:buildBasic");
  
    item1->passItem();
    NodeLabel zero(0);
    MyNetType::iterator nodeIt = netToFill.insert(zero);
    if(*(*nodeIt)!=zero)
        item1->failItem(__SPOT__);
  
    NodeLabel one(1);
    nodeIt = netToFill.insert(one);
    if(*(*nodeIt)!=one)
        item1->failItem(__SPOT__);
  
    MyNetType::arc_iterator arcIt =
        netToFill.arc_insert(netToFill[0],ArcCost(3),netToFill[1]);
    //cout<<"Arc is "<<*arcIt<<std::endl;
  
    if(arcIt == netToFill.arc_end())
        item1->failItem(__SPOT__);
  
}

static void test1()
{
    TestItem *item1 = new TestItem("StaticNetTest01:Construct and mess with labels");
    NodeLabel one(1);
    NodeLabel two(2);
    item1->passItem();
    if(two < one)
        item1->failItem(__SPOT__);
    if(one == two)
        item1->failItem(__SPOT__);
    if(!(one < two))
        item1->failItem(__SPOT__);
}

static void test2()
{
    TestItem *item1 = new TestItem("StaticNetTest02:Construct a node via label and int");
    MyNodeType node1(NodeLabel(1));
    MyNodeType node2(1); // should be the same as node 1
    if(!(node1==node2))
        item1->failItem(__SPOT__);
    else
        item1->passItem();
}

static void test3()
{
    TestItem *item1 = new TestItem("StaticNetTest03:Construct Arc Cost");
    ArcCost myCost1(1);
    ArcCost myCost2(2);
    item1->passItem();
    if(myCost1 == myCost2)
        item1->failItem(__SPOT__);
    if(myCost2 == myCost1)
        item1->failItem(__SPOT__);
    if(myCost2 < myCost1)
        item1->failItem(__SPOT__);
    if(!(myCost1 < myCost2))
        item1->failItem(__SPOT__);
}

static void test4()
{
    TestItem *item1 = new TestItem("StaticNetTest04:Construct Net Test");
  
    MyNetType net(5,5);
    net.insert( NodeLabel(1) );
    net.insert( 2 );
  
    MyNetType::arc_iterator arcIt= 
        net.arc_insert(net.find(NodeLabel(1)),ArcCost(4),net.find(NodeLabel(2)));
    if(arcIt == net.arc_end())
        item1->failItem(__SPOT__);
    else
        item1->passItem();
    //cout<<"Network Out"<<std::endl;
    //cout<<net<<std::endl;
}

static void test5()
{
    TestItem *item1 = new TestItem("StaticNetTest05:CheckSizes");
    MyNetType net(4,2);
    if(net.size()!=0||net.arc_size()!=0)
        {
        std::cout<<"Sizes n="<<net.size()<<" a="<<net.arc_size()<<std::endl;
        item1->failItem(__SPOT__);
        return;
        }
    else
        item1->passItem();
    if((*(net[0])).size()!=0)
        item1->failItem(__SPOT__);
}

int compareArcToZeroOne(MyNetType::const_arc_iterator arcIt)
{
    static const ArcCost daArc(3);
    //cout<<"comparing "<<*(*arcIt)<<" to"<< daArc<<std::endl;
    if(*(*arcIt)==daArc)
        return 0;
    return 1;
}

int compareToOne(MyNetType::const_iterator nodeIt)
{
    static const NodeLabel daNode(1);
    //  cout<<"*(*nodeIt)="<<*(*nodeIt)<<" different than "<<daNode<<std::endl;
    if(*(*nodeIt)==daNode)
        return 0;
    return 1;
}

int compareToOne(MyNetType::reverse_iterator nodeIt)
{
    static const NodeLabel daNode(1);
    if(*(*nodeIt)==daNode)
        return 0;
    return 1;
}

static void test6()
{
    TestItem *item1 = new TestItem("StaticNetTest06:Test Node Iterators");
    item1->passItem();
    MyNetType net(6,6);
  
    buildBasicNet(net);
  
    //cout<<"Net="<<net<<std::endl;
  
    MyNetType::iterator nodeIt = net.find(NodeLabel(1));
    if(compareToOne(nodeIt))
        item1->failItem(__SPOT__);
    nodeIt = net[1];
    if(compareToOne(nodeIt))
        {
        std::cout<<"On failure is :"<<*(*nodeIt)<<std::endl;
        item1->failItem(__SPOT__);
        }
    MyNetType::const_iterator nodeCIt = net.begin();
    nodeCIt++;
    if(compareToOne(nodeCIt))
        {
        std::cout<<"On failure is :"<<*(*nodeCIt)<<std::endl;
        item1->failItem(__SPOT__);
        }
    MyNetType::reverse_iterator nodeCRIt = net.rbegin();
    if(compareToOne(nodeCRIt))
        {
        std::cout<<"On failure is :"<<*(*nodeCRIt)<<std::endl;
        item1->failItem(__SPOT__);
        }
}

static void test7()
{
    TestItem *item1 = new TestItem("StaticNetTest07:Test Arc Iterators");
  
    item1->passItem();
    MyNetType net(2,1);
  
    buildBasicNet(net);
  
    int count=0;
    for(MyNetType::arc_iterator arcIt = net.arc_begin();
        arcIt != net.arc_end(); arcIt++)
        count++;
    if(count>1)
        {
        std::cout<<"Count="<<count<<std::endl;
        item1->failItem(__SPOT__);
        }
    MyNetType::const_arc_iterator arcCIt = net.arc_begin();
    if(compareArcToZeroOne(arcCIt))
        {
        std::cout<<"FAILED ARC:"<<*(*arcCIt)<<std::endl;
        item1->failItem(__SPOT__);
        }
}

static void test8()
{
    TestItem *item1 = new TestItem("StaticNetTest08:Test Other Arc Construction");
    item1->passItem();
    MyNetType net(6,6);
  
    buildBasicNetPlus(net);
    if(net.size()!= 4)
        {
        std::cout<<"reported "<<net.size()<<" should be 3"<<std::endl;
        item1->failItem(__SPOT__);
        }
    if(net.arc_size()!= 2)
        {
        std::cout<<"reported "<<net.arc_size()<<" should be 2"<<std::endl;
        item1->failItem(__SPOT__);
        }
}

static void test9()
{
    TestItem *item1 = new TestItem("StaticNetTest09:Test getNodeId functions");
    item1->passItem();
    MyNetType net(6,6);
    buildBasicNetPlus(net);
  
    MyNetType::iterator nodeIt = net.begin(); 
    NodeId zeroId(0);
    if(net.getNodeId(nodeIt)!=zeroId)
        {
        std::cout<<"***"<<net.getNodeId(nodeIt)<<"***"<<std::endl;
        item1->failItem(__SPOT__);
        }
    nodeIt++;
    if(NodeId(1)!= net.getNodeId(nodeIt))
        {
        std::cout<<"***"<<net.getNodeId(nodeIt)<<"***"<<std::endl;
        item1->failItem(__SPOT__);
        }
    nodeIt++;
    if(NodeId(2)!= net.getNodeId(nodeIt))
        {
        std::cout<<"***"<<net.getNodeId(nodeIt)<<"***"<<std::endl;
        item1->failItem(__SPOT__);
        }
    nodeIt++;
    nodeIt++;
    if(nodeIt != net.end())
        {
        std::cout<<"***"<<net.getNodeId(nodeIt)<<"***"<<std::endl;
        item1->failItem(__SPOT__);
        }
}

static void test10()
{
    TestItem *item1 = new TestItem("StaticNetTest10:Test Setting through *operator");
    item1->passItem();
    MyNetType net(5,5);
    buildBasicNetPlus(net);
  
    MyNetType::iterator nodeIt = net.find(NodeId(3));
    if(*(*nodeIt)!=0) // should be initialized to zero
        item1->failItem(__SPOT__);
    //cout<<"\n*nodeIt="<<*nodeIt<<std::endl;
    //cout<<"nodeIt="<<nodeIt<<std::endl;
    //cout<<"Setting nodeIt = 12"<<std::endl;
    *nodeIt = NodeLabel(12);
    //cout<<"*nodeIt="<<*nodeIt<<std::endl;
    //cout<<"nodeIt="<<nodeIt<<std::endl;
    if(*(*nodeIt)!=NodeLabel(12))
        {
        std::cout<<"ERROR:Value is "<<*(*nodeIt)<<std::endl;
        item1->failItem(__SPOT__);
        }
    if(*(*nodeIt)!=12) // should be similar to above
        {
        std::cout<<"ERROR:Value is "<<*(*nodeIt)<<std::endl;
        item1->failItem(__SPOT__);
        }
}
static void test11()
{
    TestItem *ti = new TestItem("StaticNetTest11:Checking arc iterators");
    ti->passItem();
    MyNetType net(4,2);
    MyNetType net1(4,2);
    buildBasicNetPlus(net);
    buildBasicNetPlus(net1);
  
    MyNetType::arc_iterator arcIt2 = net.arc_begin(); 
    for(MyNetType::iterator nodeIt1 = net.begin();
        nodeIt1 != net.end(); nodeIt1++)
        {
        for(MyNetType::Node::iterator arcIt1 = (*nodeIt1).begin();
            arcIt1 != (*nodeIt1).end();arcIt1++,arcIt2++)
            {
            if((*(*arcIt1))!=(*(*arcIt2)) )
                {
                std::cout<<"Arc1="<<(*(*arcIt1))<<std::endl;
                std::cout<<"Arc2="<<(*(*arcIt2))<<std::endl;
                std::cout<<"ERROR, arcs not equall"<<std::endl;
                ti->failItem(__SPOT__);
                }
            }
        }
}

static void test12()
{
    TestItem *ti = new TestItem("StaticNetTest12:Big Network");
    ti->passItem();
    MyNetType net(4,2);
    MyNetType net1(4,2);
    buildBasicNetPlus(net);
    buildBasicNetPlus(net1);
  
    MyNetType::arc_iterator arcIt2 = net.arc_begin(); 
    for(MyNetType::iterator nodeIt1 = net.begin();
        nodeIt1 != net.end(); nodeIt1++)
        {
        for(MyNetType::Node::iterator arcIt1 = (*nodeIt1).begin();
            arcIt1 != (*nodeIt1).end();arcIt1++,arcIt2++)
            {
            if((*(*arcIt1))!=(*(*arcIt2)) )
                {
                std::cout<<"Arc1="<<(*(*arcIt1))<<std::endl;
                std::cout<<"Arc2="<<(*(*arcIt2))<<std::endl;
                std::cout<<"ERROR, arcs not equall"<<std::endl;
                ti->failItem(__SPOT__);
                }
            }
        }
}

static void test13()
{
    TestItem *ti=new TestItem("StaticNetTest13:Multiple Node Insert, check for (expected) failed arc insert");
    ti->passItem();

    // NOTES:
    // Kind of a complex thing.  Static net with 4 nodes, 5 arcs
    // STEP 1: inserts nodes quickly, checking only the first.

    MyNetType myStaticSPNet(4,5);
    NodeLabel tmp;
    MyNetType::iterator nodeIt = myStaticSPNet.insert( tmp );// NodeId0
    if(myStaticSPNet.find(NodeId(0))!= nodeIt)
        {
        std::cout<<"ERROR:There is some silly problem"<<std::endl;
        ti->failItem(__SPOT__);
        return;
        }

    MyNetType::iterator node0 = myStaticSPNet.insert( tmp ); // NodeId1
    MyNetType::iterator node1 = myStaticSPNet.insert( tmp ); // NodeId2
    MyNetType::iterator node2 = myStaticSPNet.insert( tmp ); // NodeId3
    if(node2==myStaticSPNet.end())
        ti->failItem(__SPOT__);

    /* HPW */
    // all nodes inserted...
    // inserting first arc...
    MyNetType::arc_iterator arc1It= 
        myStaticSPNet.arc_insert(myStaticSPNet.find(NodeId(0)),
                                 ArcCost(1),
                                 myStaticSPNet.find(NodeId(1)));
    if(arc1It == (*node0).end())
        {
        ti->failItem(__SPOT__);
        return;
        }
  
    MyNetType::arc_iterator arc2It= 
        myStaticSPNet.arc_insert(myStaticSPNet.find(NodeId(1)),
                                 ArcCost(2),
                                 myStaticSPNet.find(NodeId(2)));
    if(arc2It == (*node1).end())
        {
        ti->failItem(__SPOT__);
        return;
        }

  
    // The following attempts to insert an arc later than what is allowed.
    MyNetType::iterator tail = myStaticSPNet.find(NodeId(0));
    MyNetType::iterator header = myStaticSPNet.find(NodeId(3));
  
    if(tail==myStaticSPNet.end() ||
       header==myStaticSPNet.end())
        ti->failItem(__SPOT__);
  
    //std::cout<<"*(*tail)="<<*(*tail)<<std::endl;
    //std::cout<<"*(*header)="<<*(*header)<<std::endl;
    ArcCost cost(1);
  
    MyNetType::arc_iterator arc5It= 
        myStaticSPNet.arc_insert(tail, cost, header);
  
    /* The previous call tests a StaticNet issues: insertion order 
       can't insert an arc of an earlier node later in the network insert.
       The following checks to make sure the resturn value was correct since
       the arc should be rejected...
    */
    if(arc5It == myStaticSPNet.arc_end())
        std::cerr<<"INFO: Ignore previous improper message, is intentional!"<<std::endl;
    else
        {
        std::cerr<<"(*arc5It = )"<<*(*arc5It)<<std::endl;
        std::cerr<<"End is "<<(*(*(*node1).end()))<<std::endl;
        ti->failItem(__SPOT__);
        }
}

static void test14()
{
    TestItem *ti= new TestItem("StaticNetTest14:Test arc_find");
  
    MyNetType myStaticSPNet(4,2);
    buildBasicNetPlus(myStaticSPNet);
  
    MyNetType::const_arc_iterator foundArc = 
        myStaticSPNet.arc_find(NodeId(0),NodeId(1));
    if(foundArc==myStaticSPNet.arc_end())
        ti->failItem(__SPOT__);
    else
        ti->passItem();
}

static void test15()
{
    TestItem *ti= new TestItem("StaticNetTest15:visit every part of every data structure");
  
    MyNetType myNet(4,5);
    buildBigNet(myNet);
    for(MyNetType::const_iterator mnci = myNet.begin();
        mnci!= myNet.end();mnci++)
        {
        NodeId id=myNet.getNodeId(mnci);
        //std::cout<<"From Node "<<id<<" with data ="<<*(*mnci)<<std::endl;
        for(MyNetType::const_arc_iterator mncai = (*mnci).begin();
            mncai != (*mnci).end();++mncai)
            {
            //std::cout<<" Reaching to "<<myNet.getNodeId((*mncai).head())
                //<<" with arc data "<<*(*mncai)<<std::endl;
            }
        }
    for(MyNetType::iterator mnci = myNet.begin();
        mnci!= myNet.end();mnci++)
        {
        NodeId id=myNet.getNodeId(mnci);
        //std::cout<<"From Node "<<id<<" with data ="<<*(*mnci)<<std::endl;
        for(MyNetType::const_arc_iterator mncai = (*mnci).begin();
            mncai != (*mnci).end();mncai++)
            {
           // std::cout<<"Visiting to "<<myNet.getNodeId((*mncai).head())
                //<<" with arc data "<<*(*mncai)<<std::endl;
            }
        }
    for(MyNetType::iterator mnci = myNet.begin();
        mnci!= myNet.end();mnci++)
        {
        NodeId id=myNet.getNodeId(mnci);
        //std::cout<<"From Node "<<id<<" with data ="<<*(*mnci)<<std::endl;
        for(MyNetType::const_arc_iterator mncai = (*mnci).begin();
            mncai != (*mnci).end();mncai++)
            {
            //std::cout<<"Visiting to "<<myNet.getNodeId((*mncai).head())
                //<<" with arc data "<<*(*mncai)<<std::endl;
            }
        }  
    ti->passItem();
}
static void test16()
{
    TestItem *ti= new TestItem("StaticNetTest16:checking iterators");
    StaticNet<int,int> myStaticNet(2,1);
    myStaticNet.insert(3);
    myStaticNet.insert(4);
    myStaticNet.arc_insert(myStaticNet.find(NodeId(0)),3,myStaticNet.find(NodeId(1)));
    StaticNet<int,int>::Node &myNode = *(myStaticNet.find(NodeId(0)));
    StaticNet<int,int>::Node::iterator nodeArcIt = myNode.begin();
    nodeArcIt++;
    
    StaticNet<int,int>::iterator nodeIt =   myStaticNet.find(NodeId(0));

    StaticNet<int,int>::arc_iterator ai =
        myStaticNet.arc_find(nodeIt,3);
    ti->passItem();
}

int StaticNetTest(TestBed &myBed)
{
    
    TestItem::setBed(&myBed);
  
    test1(); 
    test2(); 
    test3(); 
    test4(); 
    test5(); 
    test6(); 
    test7(); 
    test8(); 
    test9(); 
    test10(); 
    test11(); 
    test12(); 
    test13(); 
    test14(); 
    test15(); 
    test16(); 
    return 0;
}
