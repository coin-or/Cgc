/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <DynNet.h>
#include "TestData.h"

using namespace Cgc;
typedef DynNet<NodeLabel,ArcCost> MyNetType;

#include <iostream>
#include <fstream>
#include "TestItem.h"
#include "TestBed.h"

void buildBasicNet(MyNetType &netToFill)
{
  static TestItem *item1 = new TestItem("DynNetTest:aux1:buildBasic");
  //builds two nodes and one arc net...

  item1->passItem();
  NodeLabel zero(0);
  //cout<<"bb1"<<std::endl;

  MyNetType::iterator nodeIt = netToFill.insert(zero);
  if(*(*nodeIt)!=zero)
    item1->failItem(__SPOT__);
  NodeLabel one(1);
  //cout<<"bb2"<<std::endl;
  nodeIt = netToFill.insert(one);
  if(*(*nodeIt)!=one)
    item1->failItem(__SPOT__);
  ArcCost threeCost(3);
  //cout<<"bb3"<<endl;
  MyNetType::iterator head=netToFill.begin();
  MyNetType::iterator tail=head++;
  //cout<<"bb4"<<endl;
  MyNetType::const_arc_iterator arcIt =
    netToFill.arc_insert(tail,threeCost,head);
  //cout<<"bb4.5"<<endl;
  if(arcIt == netToFill.arc_end())
    item1->failItem(__SPOT__);
  if(netToFill.arc_size()!=1)
    item1->failItem(__SPOT__);
  if(netToFill.size()!=2)
    item1->failItem(__SPOT__);
  //cout<<"bb5"<<endl;
}

static void test1()
{
  TestItem *item1 = new TestItem("DynNetTest01:Construct and mess with labels");
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
  TestItem *item1 = new TestItem("DynNetTest02:Construct a node via label and int");
  MyNetType::Node node1(NodeLabel(1));
  MyNetType::Node node2(1); // should be the same as node 1
  if((*node1)!=(*node2))
    {
      std::cout<<"*Node1="<<*node1<<" , *node2="<<*node2<<std::endl;
      item1->failItem(__SPOT__);
    }
  else
    item1->passItem();
}

static void test3()
{
  TestItem *item1 = new TestItem("DynNetTest03:Construct Arc Cost");
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
  TestItem *item1 = new TestItem("DynNetTest04:Construct Net Test");

  MyNetType net(5,5);
  net.insert( NodeLabel(1) );
  net.insert( 2 );

  MyNetType::iterator tail = net.find(NodeLabel(1));
  MyNetType::iterator head = net.find(NodeLabel(2));
  MyNetType::const_arc_iterator arcIt= 
    net.arc_insert(tail,ArcCost(4),
		   head);
  if(arcIt == MyNetType::const_arc_iterator(net.arc_end()))
    item1->failItem(__SPOT__);
  else
    item1->passItem();
  //std::cout<<"Network Out"<<endl;
  //std::cout<<net<<endl;
}

static void test5()
{
  TestItem *item1 = new TestItem("DynNetTest05:CheckSizes");
  MyNetType net(0,0);
  if(net.size()!=0||net.arc_size()!=0)
    {
      std::cout<<"Num nodes = "<<net.size()<<" and should be 0"<<std::endl;
      std::cout<<"Num arcs = "<<net.arc_size()<<" and should be 0"<<std::endl;
      item1->failItem(__SPOT__);
    }
  else
    item1->passItem();
}

int compareArcToZeroOne(MyNetType::Node::iterator arcIt)
{
  static TestItem *item1 = 
    new TestItem("DynNetTest:aux2:Node::iterator compareToZeroOne");
  // these never fail, but should be called once.
  item1->passItem();
  static const ArcCost daArc(3);
  if((*(*arcIt))==daArc)
    return 0;
  //std::cout<<"daArc="<<daArc<<endl;
  //std::cout<<"*arcIt="<<*(*arcIt)<<endl;
  return 1;
}

int compareArcToZeroOne(MyNetType::const_arc_iterator arcIt)
{
  static TestItem *item1 = 
    new TestItem("DynNetTest:aux3:arc_iterator compareToZeroOne");
  // these never fail, but should be called once.
  item1->passItem();
  static const ArcCost daArc(3);
  if((*(*arcIt))==daArc)
    return 0;
  //std::cout<<"daArc="<<daArc<<endl;
  //std::cout<<"*arcIt="<<*(*arcIt)<<endl;
  return 1;
}

int compareToOne(MyNetType::const_iterator nodeIt)
{
  static const NodeLabel daNode(1);
  if(*(*nodeIt)==daNode)
    return 0;
  return 1;
}

/*
int compareToOne(MyNetType::reverse_iterator nodeIt)
{
  static const NodeLabel daNode(1);
  if(*(*nodeIt)==daNode)
    return 0;
  return 1;
}
*/

static void test6()
{
  TestItem *item1 = new TestItem("DynNetTest06:Test Node Iterators");
  item1->passItem();
  MyNetType net(0,0);
  //std::cout<<"6.1"<<endl;

  buildBasicNet(net);

  std::cout<<"Net="<<net<<std::endl;

  MyNetType::iterator nodeIt = net.find(NodeLabel(1));
  if(compareToOne(nodeIt))
    {
      std::cout<<"FAILED node =>"<<*nodeIt<<std::endl;
      item1->failItem(__SPOT__);
    }
  //std::cout<<"6.2"<<endl;
  nodeIt = net.begin();nodeIt++;
  if(compareToOne(nodeIt))
    {
      const MyNetType::Node &nodeRef = (*nodeIt);
      std::cout<<"On failure is :";
      //nodeRef.print(std::cout);
      std::cout<<nodeRef;
      std::cout<<std::endl;
      item1->failItem(__SPOT__);
    }
  //std::cout<<"6.3"<<endl;
  MyNetType::const_iterator nodeCIt = net.begin();
  nodeCIt++;
  if(compareToOne(nodeCIt))
    {
      const MyNetType::Node &nodeRef = (*nodeIt);
      std::cout<<"On failure is :"<<nodeRef;
      //nodeRef.print(std::cout);
      std::cout<<std::endl;
      item1->failItem(__SPOT__);
    }
  //std::cout<<"6.4"<<std::endl;
  /*
  MyNetType::reverse_iterator nodeCRIt = net.rbegin();
  if(compareToOne(nodeCRIt))
    {
      const MyNetType::Node &nodeRef = (*nodeIt);
      std::cout<<"On failure is :";
      nodeRef.print(std::cout);
      std::cout<<std::endl;
      item1->failItem(__SPOT__);
    }
  */
}

static void test7()
{
  TestItem *item1 = new TestItem("DynNetTest07:Test Arc Iterators");

  item1->passItem();
  MyNetType net(0,0);
  //std::cout<<"7.1"<<std::endl;

  buildBasicNet(net);

  int  count=0;
  //std::cout<<"7.2"<<std::endl;
  std::cout<<"Net Constructed, iterating"<<std::endl<<net<<std::endl;
  //std::cout<<"7.2.1"<<std::endl;
  for(MyNetType::arc_iterator arcIt = net.arc_begin();
      arcIt != net.arc_end(); arcIt++)
    {
      count++;
      //std::cout<<"7.2.2"<<std::endl;
      std::cout<<"arc # "<<count<<" = "<<*(*arcIt)<<std::endl;
      //std::cout<<"7.2.3"<<std::endl;
      if(count>1)
	{
	  std::cout<<"counted "<<count<<" arcs...breaking"<<std::endl;
	  item1->failItem(__SPOT__);
	  break;
	}
      //std::cout<<"7.2.4"<<std::endl;
    }
  //std::cout<<"7.3"<<std::endl;
  MyNetType::const_arc_iterator arcCIt(net.arc_begin());
  if(compareArcToZeroOne(arcCIt))
    {
      std::cout<<"FAILED ARC:"<<*arcCIt<<std::endl;
      item1->failItem(__SPOT__);
    }

  MyNetType::iterator bbb = net.begin();
  const MyNetType::Node &node = *bbb;
  typedef MyNetType::Node NetNode;
  NetNode::iterator nodeArcIter = node.begin();
  if(compareArcToZeroOne(nodeArcIter))
    {
      std::cout<<"FAILED ARC:"<<*arcCIt<<std::endl;
      item1->failItem(__SPOT__);
    }
  size_t arcCnt=node.size();
  for(NetNode::iterator nodeArcIter= node.begin();
      nodeArcIter != node.end();nodeArcIter++)
    {
      std::cout<<"*nodeArcIter="<<(*(*nodeArcIter))<<std::endl;
      arcCnt--;
    }
  if(arcCnt)
    item1->failItem(__SPOT__);
  //std::cout<<"7.4"<<std::endl;
}

static void test8()
{
  TestItem *item1 = new TestItem("DynNetTest08:Test Node's Arc Iterators");

  MyNetType net(0,0);
  buildBasicNet(net);

  
  for(MyNetType::const_iterator nodeIt = net.begin();
      nodeIt != net.end();nodeIt++)
    {
      //std::cout<<"At Node :"<<*nodeIt<<std::endl;

      int count=0;
      for(MyNetType::Node::const_iterator aIt = (*nodeIt).begin();
	  aIt != (*nodeIt).end();aIt++)
	{
	  std::cout<<" **Arc at node has :"<<*aIt<<std::endl;
	  count++;
	  if(count>1)
	    {
	      item1->failItem(__SPOT__);
	      break;
	    }
	}
      count=0;
      
      //MyNetType::const_arc_iterator itBegin(net,nodeIt,(*nodeIt).begin());
      //MyNetType::const_arc_iterator itEnd(net,nodeIt,(*nodeIt).end());
      MyNetType::const_arc_iterator itEnd = net.arc_end();
      for(MyNetType::const_arc_iterator arcIt2(net,nodeIt);
	  arcIt2 != itEnd;arcIt2++)
	{
	  //std::cout<<" **2nd method Arc at node has :"<<*arcIt2<<std::endl;
	  count++;
	  if(count>1)
	    {
	      item1->failItem(__SPOT__);
	      break;
	    }
	}
      count=0;
      
      std::cout<<"ArcSize = "<<net.arc_size()<<std::endl;
      MyNetType::const_arc_iterator endIt = net.arc_end();
      for(MyNetType::const_arc_iterator arcIt3 = net.arc_begin();
	  arcIt3 != endIt ;arcIt3++)
	{
	  //std::cout<<" **3rd method Arc at node has :"<<*arcIt3<<std::endl;
	  count++;
	  if(count>1)
	    {
	      item1->failItem(__SPOT__);
	      break;
	    }
	}
    }
  item1->passItem();
}

static void test9()
{
    TestItem *ti = new TestItem("DynNetTest09:Arc Iterators more and arc iter compares");

  MyNetType net(0,0);
  buildBasicNet(net);

  MyNetType::arc_iterator ait(net);
  MyNetType::arc_iterator ait2(net,net.begin());
  MyNetType::arc_iterator stopper(net,net.end());
  if(ait!=ait2)
    ti->failItem(__SPOT__);
  if(ait==ait2)
    ti->passItem();
  int count=0;
  if(ait2 == stopper)
    ti->failItem(__SPOT__);
  while(ait2 != stopper)
    {
      count++;
      ait2++;
      if(count>1)
	break;
    }
  if(count != 1)
    ti->failItem(__SPOT__);
}

static void test10()
{
  TestItem *ti = new TestItem("DynNetTest10:Arc Iterators 3 ");

  MyNetType net(0,0);
  buildBasicNet(net);
  NodeLabel two(2);
  MyNetType::iterator node2= net.insert(two);
  MyNetType::iterator node0= net.find(NodeId(0));
  ArcCost four(4);
  net.arc_insert(node0,four,node2);
  // now see that node iterates over them correctly
  if((*node0).size()==2)
    ti->passItem();
  else
    {
      ti->failItem(__SPOT__);
      std::cout<<"Incorrect reporting of number of arcs in Node"<<std::endl;
    }
  int counter=0;
  std::cout<<"Begin at "<<*(*(*node0).begin())<<std::endl;
  MyNetType::Node::iterator nodeArcIt = (*node0).begin();
  for( ;nodeArcIt != (*node0).end(); nodeArcIt++)
    {
      counter++;
      if(counter>2)
	break;
      std::cout<<"*(*arcIt)="<<*(*nodeArcIt)<<std::endl;
    }
  if(counter!=2)
    {
      std::cout<<"Incorrect iterating of arcs in Node"<<std::endl;
      ti->failItem(__SPOT__);
    }
}

static void test11()
{
  TestItem *ti = new TestItem("DynNetTest11: Delete Node test");
  MyNetType net(0,0);
  size_t startNodes;
  size_t startArcs;

  buildBasicNet(net);
  startNodes = net.size();
  startArcs = net.arc_size();

  std::cout<<"Net="<<net<<std::endl;

  net.erase(net.begin());
  std::cout<<"Node 0 removed, Net="<<net<<std::endl;
  if(net.size()!= startNodes-1)
    {
      std::cout<<"start="<<startNodes<<" newSize="<<net.size()<<std::endl;
      ti->failItem(__SPOT__);
    }
  // should have removed the only arc as well.
  if(net.arc_size()!=0)
    {
      std::cout<<" newArcSize="<<net.arc_size()<<" expected zero!"<<std::endl;
      ti->failItem(__SPOT__);
    }
  ti->passItem();
  
}

void printTailHead(MyNetType &net, 
		   MyNetType::iterator &tail, MyNetType::iterator &head)
{
  std::cout<<"Arc goes from "<<net.getNodeId(tail)<<" to "<<net.getNodeId(head)<<std::endl;
}

void buildBiggerNet(MyNetType &netToFill)
{
  static TestItem *ti = new TestItem("DynNetTest:aux: buildBiggerNet");
  //builds a 4 node and 5 arc net...
  
  NodeLabel zero(0);

  MyNetType::iterator nodeIt = netToFill.insert(zero);
  NodeLabel one(1);
  nodeIt = netToFill.insert(one);
  NodeLabel two(2);
  nodeIt = netToFill.insert(two);
  NodeLabel three(3);
  nodeIt = netToFill.insert(three);

  ArcCost oneCost(1);
  ArcCost twoCost(2);
  ArcCost threeCost(3);
  ArcCost fourCost(4);
  ArcCost fiveCost(5);
  MyNetType::iterator head=netToFill.begin();
  MyNetType::iterator tail=head++;
  MyNetType::arc_iterator theArc(netToFill);
  
  // insert an arc from zero to one
  printTailHead(netToFill,tail,head);
  theArc=netToFill.arc_insert(tail,oneCost,head);
  if(theArc==netToFill.arc_end())
    ti->failItem(__SPOT__);
  std::cout<<"after added first arc:\n"<<netToFill<<std::endl;
  head++;
  // insert an arc from zero to two
  printTailHead(netToFill,tail,head);
  theArc = netToFill.arc_insert(tail,twoCost,head);
  if(theArc==netToFill.arc_end())
    ti->failItem(__SPOT__);
  std::cout<<"after added second arc:\n"<<netToFill<<std::endl;
  tail++;
  // insert an arc from one to two
  printTailHead(netToFill,tail,head);
  theArc = netToFill.arc_insert(tail,threeCost,head);
  if(theArc==netToFill.arc_end())
    ti->failItem(__SPOT__);
  std::cout<<"after added third arc:\n"<<netToFill<<std::endl;
  head++;tail++;
  //insert an arc from two to three
  printTailHead(netToFill,tail,head);
  theArc = netToFill.arc_insert(tail,fourCost,head);
  if(theArc==netToFill.arc_end())
    ti->failItem(__SPOT__);
  std::cout<<"after added fourth arc:\n"<<netToFill<<std::endl;

  //insert an arc from zero to three
  tail=netToFill.begin();
  printTailHead(netToFill,tail,head);
  theArc = netToFill.arc_insert(tail,fiveCost,head);
  if(theArc==netToFill.arc_end())
    ti->failItem(__SPOT__);
  std::cout<<"after added fifth arc:\n"<<netToFill<<std::endl;
  ti->passItem();
}

static void test12()
{
  TestItem *ti = new TestItem("DynNetTest12: Delete Arc Test");
  MyNetType net(0,0);
  size_t startNodes;
  size_t startArcs;

  buildBiggerNet(net);

  startNodes = net.size();
  startArcs = net.arc_size();

  std::cout<<"Net="<<net<<std::endl;

  MyNetType::iterator nodeIt = net.begin();
  nodeIt++;
  
  net.erase(nodeIt);
  std::cout<<"Node 1 removed, Net="<<net<<std::endl;
  
  if(net.size()!= startNodes-1)
    {
      std::cout<<"start="<<startNodes<<" newSize="<<net.size()<<std::endl;
      ti->failItem(__SPOT__);
    }
  if(net.size()!=3)
    {
      std::cout<<"start="<<startNodes<<" newSize="<<net.size()<<std::endl;
      ti->failItem(__SPOT__);
    }

  // should have removed two arcs
  if(net.arc_size()!=startArcs-2)
    {
      std::cout<<__LINE__<<" newArcSize="<<net.arc_size()<<" expected !"<<startArcs-2<<std::endl;
      ti->failItem(__SPOT__);
    }
  if(net.arc_size()!=3)
    {
      std::cout<<" newArcSize="<<net.arc_size()<<" expected 3!"<<std::endl;
      ti->failItem(__SPOT__);
    }

  MyNetType::arc_iterator last = net.arc_begin();
  last++;
  net.arc_erase(net.arc_begin(),last);
  
  if(net.arc_size()!=2)
    {
      std::cout<<" newArcSize="<<net.arc_size()<<" expected 3!"<<std::endl;
      ti->failItem(__SPOT__);
    }
  std::cout<<"First arc removed, Net="<<net<<std::endl;
  ti->passItem();
  
}
static void test13()
{
  TestItem *ti = new TestItem("DynNetTest13: Save and load Adjacency");
  MyNetType net(0,0);
  //unsigned int startNodes;
  //unsigned int startArcs;

  buildBiggerNet(net);
  {
    std::ofstream testFile("adjnet.net");
    net.saveAdjacency(testFile);
    testFile.close();
    std::cout<<"13.1.net="<<net<<std::endl;
  }
  {
    std::cout<<"13.1.net="<<net<<std::endl;
    std::ifstream testFile("adjnet.net");
    net.loadAdjacency(testFile);
    testFile.close();
    std::cout<<"13.2.net="<<net<<std::endl;
    std::ofstream testFile2("adjnet2.net");
    net.saveAdjacency(testFile2);
    testFile2.close();
  }
  ti->passItem();
}
static void test14()
{
  TestItem *item1 = new TestItem("DynNetTest14:Test Node's Back Arc Iterators");

  MyNetType net(0,0);
  buildBasicNet(net);

  
  for(MyNetType::const_iterator nodeIt = net.begin();
      nodeIt != net.end();nodeIt++)
    {
      int count=0;
      for(MyNetType::Node::const_back_iterator bIt = (*nodeIt).back_begin();
	  bIt != (*nodeIt).back_end();bIt++)
	{
	  count++;
	  if(count>1)
	    {
	      item1->failItem(__SPOT__);
	      return;
	    }
	}
      if(nodeIt != net.begin()&&count==0)
	{
	  item1->failItem(__SPOT__);
	  return;
	}
    }
  item1->passItem();
}

static void test15()
{
  TestItem *ti = new TestItem("DynNetTest15:Test calling functions on stored types");
  MyNetType net(0,0);
  buildBasicNet(net); // 2 nodes, one arc network

  const int testval=12345;
  // get the first node using a Net::iterator
  MyNetType::iterator nodeIt = net.begin();
  // refer to the Node referred to by the iterator
  //ti->failItem(__SPOT__);  // The cost_cast should not be required.
  MyNetType::Node &nodeRef = const_cast<MyNetType::Node &>(*nodeIt);
  // refer to the user data contained in the Node
  NodeLabel &userData = (*nodeRef);
  // set the value through a user data reference.
  userData.setVal(testval);

  // compare the value through the iterator to the test value
  if((*(*nodeIt)).getVal()==testval)
    ti->passItem();
  else
    ti->failItem(__SPOT__);
  
  MyNetType::iterator nodeIt2 = nodeIt;
  nodeIt2++; // go to the last node, peek back to the arc
  MyNetType::Node::back_iterator bit = (*nodeIt2).back_begin();
  if(bit == (*nodeIt2).back_end())
    {
      ti->failItem(__SPOT__);
      return;
    }
  
  // convert to net-level arc iterator
  //std::cout<<"NodeIt points to "<<*nodeIt<<std::endl;
  //std::cout<<"NodeIt numArcs ="<<(*nodeIt).size()<<std::endl;
  //std::cout<<"netaddy="<<&net<<std::endl;
  //std::cout<<"nodeaddy="<<&(*nodeIt)<<std::endl;
  //std::cout<<"user arc data at arcaddy ="<<&(*(*((*nodeIt).begin())))<<std::endl;
  //std::cout<<"user arc data at arcaddy2="<<&(*(*(net.arc_begin())))<<std::endl;
  MyNetType::Node::iterator nni = (*nodeIt).begin();
  //std::cout<<"user arc data at arcaddy3="<<&(*(*(nni)))<<std::endl;

  MyNetType::arc_iterator net_arc_it(net,nodeIt,(*nodeIt).begin());
  MyNetType::arc_iterator net_arc_it4(net,nodeIt,nni);
  
  MyNetType::arc_iterator net_arc_it3= 
    MyNetType::arc_iterator(net,nodeIt,(*nodeIt).begin());
  
  MyNetType::arc_iterator net_arc_it2 = net.arc_begin();
  // Get a reference to the Arc type referred to by the iterator
  MyNetType::Arc &arc= (*bit);
  // get a reference to the user data type
  ArcCost &usrCost =  (*arc);
  usrCost.setVal(testval);
  if((*(*bit)).getVal()!=usrCost.getVal())
    {
      std::cout<<"bit.getVal="<<(*(*bit)).getVal()<<std::endl;
      std::cout<<"usrCost.getVal="<<usrCost.getVal()<<std::endl;
      ti->failItem(__SPOT__);
      return;
    }

  // the following will fail if buildBasicNet changes
  if((*(*bit)).getVal()!= (*(*net_arc_it2)).getVal())
    {
      std::cout<<"ERROR:bit.getVal="<<(*(*bit)).getVal()<<std::endl;
      std::cout<<"ERROR:net_arc_it.getVal="<<(*(*net_arc_it)).getVal()<<std::endl;
      ti->failItem(__SPOT__);
      return;
    }

  if((*(*bit)).getVal()!= (*(*net_arc_it4)).getVal())
    {
      std::cout<<"ERROR:bit.getVal="<<(*(*bit)).getVal()<<std::endl;
      std::cout<<"ERROR:net_arc_it.getVal="<<(*(*net_arc_it)).getVal()<<std::endl;
      ti->failItem(__SPOT__);
      return;
    }
  if((*(*bit)).getVal()!= (*(*net_arc_it)).getVal())
    {
      std::cout<<"ERROR:bit.getVal="<<(*(*bit)).getVal()<<std::endl;
      std::cout<<"ERROR:net_arc_it.getVal="<<(*(*net_arc_it)).getVal()<<std::endl;
      ti->failItem(__SPOT__);
      return;
    }
  if((*(*bit)).getVal()!= (*(*net_arc_it3)).getVal())
    {
      std::cout<<"ERROR:bit.getVal="<<(*(*bit)).getVal()<<std::endl;
      std::cout<<"ERROR:net_arc_it.getVal="<<(*(*net_arc_it)).getVal()<<std::endl;
      ti->failItem(__SPOT__);
      return;
    }
}

static void test16()
{
  
  TestItem *ti = new TestItem("DynNetTest16:Lookup arc");
  MyNetType net(0,0);
  ArcCost threeCost(3);

  buildBiggerNet(net);
  // find the arc from node 1 to node 2
  MyNetType::const_arc_iterator anArc = net.arc_find(NodeId(1),NodeId(2));
  if(anArc==net.arc_end())
    ti->failItem(__SPOT__);
  if((*anArc).head()!=net.find(NodeId(2)))
    ti->failItem(__SPOT__);
  if((*anArc).tail()!=net.find(NodeId(1)))
    ti->failItem(__SPOT__);
  if((*(*anArc))==threeCost)
    ti->passItem();
}

 
int DynNetTest(TestBed &myBed)
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
