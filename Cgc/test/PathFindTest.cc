/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <PathFind.h>
#include <StaticNet.h>
#include <StaticFBNet.h>
#include <DynNet.h>

#include "TestItem.h"
#include "TestBed.h"
#include <string>

typedef int NodeType;
typedef int ArcType;
typedef StaticNet<NodeType,ArcType> MyNetType;
typedef DynNet<NodeType,ArcType> MyNetType2;
typedef StaticFBNet<NodeType,ArcType> MyNetType3;

std::ostream &operator<<(std::ostream &os, const MyNetType &net)
{
  os<<"My Net has "<<net.size()
      <<" nodes and "<<net.arc_size()<<" arcs."<<std::endl;
  return os;
}
/*
std::ostream &operator<<(std::ostream &os, const JustAPath &path)
{
  os<<'['<<path.size()<<"]:";
  for(JustAPath::const_iterator pathIt = path.begin();
      pathIt != path.end(); pathIt++)
    os<<(*pathIt)<<':';
  return os;
}
std::ostream &operator<<(std::ostream &os, const JustPaths &paths)
{
  os<<"Paths contains "<<paths.size()<<" paths"<<std::endl;
  for(JustPaths::const_iterator pathIt = paths.begin();
      pathIt != paths.end(); pathIt++)
      os<<(*pathIt)<<std::endl;
  return os;
}
*/

template <class NetType >
void buildSimpleNet(NetType &netToFill)
{
  netToFill.insert(0);
  netToFill.insert(1);
  netToFill.insert(2);
  netToFill.insert(3);
  netToFill.insert(4);
  netToFill.insert(5);
  netToFill.arc_insert(netToFill.find(0),0,netToFill.find(1));
  netToFill.arc_insert(netToFill.find(1),0,netToFill.find(2));
  netToFill.arc_insert(netToFill.find(1),0,netToFill.find(3));
  netToFill.arc_insert(netToFill.find(1),0,netToFill.find(4));
  netToFill.arc_insert(netToFill.find(2),0,netToFill.find(4));
  netToFill.arc_insert(netToFill.find(2),0,netToFill.find(5));
  netToFill.arc_insert(netToFill.find(3),0,netToFill.find(4));
  netToFill.arc_insert(netToFill.find(4),0,netToFill.find(5));
  //std::cout<<netToFill<<std::endl;
}
template <class NetType>
void findPathTest(const std::string &testName)
{
  TestItem *ti = new TestItem(testName.c_str());
  NetType myNet(100,100);
  buildSimpleNet(myNet);
  NetType::iterator backOne = myNet.end();
  backOne--;
  PathFind<NetType> myFind(myNet,myNet.begin(),backOne);
  JustPaths paths;
  myFind.getPaths(paths);
  //std::cout<<paths<<std::endl;
  if(paths.size()!=4)
    ti->failItem(__SPOT__);
  else
    ti->passItem();
}

int PathFindTest(TestBed &myBed)
{
  TestItem::setBed(&myBed);
  findPathTest<MyNetType>("PathFindTest:1:Find all paths :StaticNet");
  findPathTest<MyNetType2>("PathFindTest:1:Find all paths :DynNet");
  findPathTest<MyNetType3>("PathFindTest:1:Find all paths :StaticFBNet");
  return 0;
}
