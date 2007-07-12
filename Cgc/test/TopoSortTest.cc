#include <TopoSort.h>

#include <StaticNet.h>
#include <DynNet.h>
#include <StaticFBNet.h>
#include <string>
#include "TestItem.h"
#include "TestBed.h"

using namespace Cgc;
struct NodeType {};
struct ArcType {};
typedef StaticNet<NodeType,ArcType>  MyNetType;
typedef DynNet<NodeType,ArcType>     MyNetType2;
typedef StaticFBNet<NodeType,ArcType> MyNetType3;

template<class NetType> 
NetType *constructNet()
{
    NetType *retval = new NetType(12,18);
    NodeType n;
    ArcType a;
    retval->insert(n);//0
    retval->insert(n);//1
    retval->insert(n);//2
    retval->insert(n);//3
    retval->insert(n);//4
    retval->insert(n);//5
    retval->insert(n);//6
    retval->insert(n);//7
    retval->insert(n);//8
    retval->insert(n);//9
    retval->insert(n);//10
    retval->insert(n);//11
    retval->arc_insert(retval->find(NodeId(0)),a,retval->find(NodeId(1)));  // 0 - 1
    retval->arc_insert(retval->find(NodeId(0)),a,retval->find(NodeId(2)));  // 0 - 2
    retval->arc_insert(retval->find(NodeId(0)),a,retval->find(NodeId(4)));  // 0 - 4
    retval->arc_insert(retval->find(NodeId(1)),a,retval->find(NodeId(2)));  // 1 - 2
    retval->arc_insert(retval->find(NodeId(1)),a,retval->find(NodeId(5)));  // 1 - 5
    retval->arc_insert(retval->find(NodeId(2)),a,retval->find(NodeId(5)));  // 2 - 5
    retval->arc_insert(retval->find(NodeId(2)),a,retval->find(NodeId(8)));  // 2 - 8
    retval->arc_insert(retval->find(NodeId(3)),a,retval->find(NodeId(2)));  // 3 - 2
    retval->arc_insert(retval->find(NodeId(4)),a,retval->find(NodeId(3)));  // 4 - 3
    retval->arc_insert(retval->find(NodeId(5)),a,retval->find(NodeId(7)));  // 5 - 7
    retval->arc_insert(retval->find(NodeId(6)),a,retval->find(NodeId(11))); // 6 - 11
    retval->arc_insert(retval->find(NodeId(7)),a,retval->find(NodeId(9)));  // 7 - 9
    retval->arc_insert(retval->find(NodeId(7)),a,retval->find(NodeId(10))); // 7 - 10
    retval->arc_insert(retval->find(NodeId(8)),a,retval->find(NodeId(10))); // 8 - 10
    retval->arc_insert(retval->find(NodeId(8)),a,retval->find(NodeId(11))); // 8 - 11
    retval->arc_insert(retval->find(NodeId(9)),a,retval->find(NodeId(6)));  // 9 - 6
    retval->arc_insert(retval->find(NodeId(9)),a,retval->find(NodeId(11))); // 9 - 11
    retval->arc_insert(retval->find(NodeId(10)),a,retval->find(NodeId(11)));// 10 - 11
    return retval;
}

template<class NetType>
void topoSortTest1(std::string testName)
{
    TestItem *ti=new TestItem(testName.c_str());
    NetType *nt = constructNet<NetType>();
    TopoSort<NetType> *sorter = new TopoSort<NetType>();
    std::vector<int> outVect;
    if(!sorter->solve(*nt,outVect))
    {
        ti->failItem(__SPOT__);
    }
    for(int cnt=0;cnt<nt->size();cnt++)
    {
        switch(cnt)
        {
        case 0:
            {
            if(outVect[0] != 0 )
                ti->failItem(__SPOT__);
            }
            break;
        case 1:
            if( outVect[1] <= outVect[0] )
                ti->failItem(__SPOT__);
            break;
        case 2:
            if(outVect[2]<=outVect[3])
                ti->failItem(__SPOT__);
            if(outVect[2]<=outVect[0])
                ti->failItem(__SPOT__);
            if(outVect[2]<=outVect[1])
                ti->failItem(__SPOT__);
            break;
        case 3:
            if(outVect[3]<=outVect[4])
                ti->failItem(__SPOT__);
            break;
        case 4:
            if(outVect[4]<=outVect[0])
                ti->failItem(__SPOT__);
            break;
        case 5:
            if(outVect[5] <= outVect[2])
                ti->failItem(__SPOT__);
            if(outVect[5] <= outVect[1])
                ti->failItem(__SPOT__);
            break;
        case 6:
            if(outVect[6]<= outVect[9])
                ti->failItem(__SPOT__);
            break;
        case 7:
            if(outVect[7]<=outVect[5])
                ti->failItem(__SPOT__);
            break;
        case 8:
            if(outVect[8]<=outVect[2])
                ti->failItem(__SPOT__);
            break;
        case 9:
            if(outVect[9]<=outVect[7])
                ti->failItem(__SPOT__);
            break;
        case 10:
            if(outVect[10]<=outVect[7])
                ti->failItem(__SPOT__);
            if(outVect[10]<=outVect[8])
                ti->failItem(__SPOT__);
            break;
        case 11:
            if(outVect[11]<=outVect[6])
                ti->failItem(__SPOT__);
            if(outVect[11]<=outVect[8])
                ti->failItem(__SPOT__);
            if(outVect[11]<=outVect[9])
                ti->failItem(__SPOT__);
            if(outVect[11]<=outVect[10])
                ti->failItem(__SPOT__);
            break;
        default:
            ti->failItem(__SPOT__);
            break;
        };
    }

    ti->passItem();
}

template<class NetType> 
NetType *constructNetCycle()
{
    NetType *retval = new NetType(4,4);
    NodeType n;
    ArcType a;
    retval->insert(n);//0
    retval->insert(n);//1
    retval->insert(n);//2
    retval->insert(n);//3
    retval->arc_insert(retval->find(NodeId(0)),a,retval->find(NodeId(1)));  // 0 - 1
    retval->arc_insert(retval->find(NodeId(1)),a,retval->find(NodeId(2)));  // 1 - 2
    retval->arc_insert(retval->find(NodeId(2)),a,retval->find(NodeId(0)));  // 2 - 0
    retval->arc_insert(retval->find(NodeId(3)),a,retval->find(NodeId(0)));  // 2 - 0
    return retval;
}

template<class NetType>
void topoSortTest2(std::string testName)
{
    TestItem *ti=new TestItem(testName.c_str());
    NetType *nt = constructNetCycle<NetType>();
    TopoSort<NetType> *sorter = new TopoSort<NetType>();
    std::vector<int> outVect;
    // should return false.  this graph can't work.
    if(sorter->solve(*nt,outVect))
    {
        ti->failItem(__SPOT__);
    }

    ti->passItem();
}

void TopoSortTest(TestBed &myBed)
{
    TestItem::setBed(&myBed);
    topoSortTest1<MyNetType>("TopoSortTest:01:build and solve one Topological sort:StaticNet");
    topoSortTest1<MyNetType2>("TopoSortTest:01:build and solve one Topological sort:DynNet");
    topoSortTest1<MyNetType3>("TopoSortTest:01:build and solve one Topological sort:StaticFBNet");
    topoSortTest2<MyNetType>("TopoSortTest:02:detect Cycles:StaticNet");
    topoSortTest2<MyNetType2>("TopoSortTest:01:detect Cycles:DynNet");
    topoSortTest2<MyNetType3>("TopoSortTest:01:detect Cycles:StaticFBNet");
}