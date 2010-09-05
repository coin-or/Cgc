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

// Creates a graph with a few quirks.  Nothing really special.
template<class NetType> 
NetType *constructNet()
{
    NetType *retval = new NetType(12,18);
    NodeType n;
    ArcType a;
    typename NetType::iterator nodeIter[12];
    nodeIter[0] = retval->insert(n);//0
    nodeIter[1] = retval->insert(n);//1
    nodeIter[2] = retval->insert(n);//2
    nodeIter[3] = retval->insert(n);//3
    nodeIter[4] = retval->insert(n);//4
    nodeIter[5] = retval->insert(n);//5
    nodeIter[6] = retval->insert(n);//6
    nodeIter[7] = retval->insert(n);//7
    nodeIter[8] = retval->insert(n);//8
    nodeIter[9] = retval->insert(n);//9
    nodeIter[10] = retval->insert(n);//10
    nodeIter[11] = retval->insert(n);//11
    retval->arc_insert(nodeIter[0],a,nodeIter[1]);  // 0 - 1
    retval->arc_insert(nodeIter[0],a,nodeIter[2]);  // 0 - 2
    retval->arc_insert(nodeIter[0],a,nodeIter[4]);  // 0 - 4
    retval->arc_insert(nodeIter[1],a,nodeIter[2]);  // 1 - 2
    retval->arc_insert(nodeIter[1],a,nodeIter[5]);  // 1 - 5
    retval->arc_insert(nodeIter[2],a,nodeIter[5]);  // 2 - 5
    retval->arc_insert(nodeIter[2],a,nodeIter[8]);  // 2 - 8
    retval->arc_insert(nodeIter[3],a,nodeIter[2]);  // 3 - 2
    retval->arc_insert(nodeIter[4],a,nodeIter[3]);  // 4 - 3
    retval->arc_insert(nodeIter[5],a,nodeIter[7]);  // 5 - 7
    retval->arc_insert(nodeIter[6],a,nodeIter[11]); // 6 - 11
    retval->arc_insert(nodeIter[7],a,nodeIter[9]);  // 7 - 9
    retval->arc_insert(nodeIter[7],a,nodeIter[10]); // 7 - 10
    retval->arc_insert(nodeIter[8],a,nodeIter[10]); // 8 - 10
    retval->arc_insert(nodeIter[8],a,nodeIter[11]); // 8 - 11
    retval->arc_insert(nodeIter[9],a,nodeIter[6]);  // 9 - 6
    retval->arc_insert(nodeIter[9],a,nodeIter[11]); // 9 - 11
    retval->arc_insert(nodeIter[10],a,nodeIter[11]);// 10 - 11
    return retval;
}

// Tests a 'successful' solve.
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
    // Check to see that nodes at the heads of arcs are later in the sequence
    // than their predecessors.
    for(unsigned int cnt=0;cnt<nt->size();cnt++)
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

// construct a simple graph with a simple cycle.
template<class NetType> 
NetType *constructNetCycle()
{
    NetType *retval = new NetType(4,4);
    NodeType n;
    ArcType a;
    typename NetType::iterator nodeIter[4];
    nodeIter[0] = retval->insert(n);//0
    nodeIter[1] = retval->insert(n);//1
    nodeIter[2] = retval->insert(n);//2
    nodeIter[3] = retval->insert(n);//3
    retval->arc_insert(nodeIter[0],a,nodeIter[1]);  // 0 - 1
    retval->arc_insert(nodeIter[1],a,nodeIter[2]);  // 1 - 2
    retval->arc_insert(nodeIter[2],a,nodeIter[0]);  // 2 - 0
    retval->arc_insert(nodeIter[3],a,nodeIter[0]);  // 2 - 0
    return retval;
}

// test the "detected cycle".
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
    // test with all networks, and a valid network.
    topoSortTest1<MyNetType>("TopoSortTest:01:build and solve one Topological sort:StaticNet");
    topoSortTest1<MyNetType2>("TopoSortTest:01:build and solve one Topological sort:DynNet");
    topoSortTest1<MyNetType3>("TopoSortTest:01:build and solve one Topological sort:StaticFBNet");
    // test with all networks, and a cyclic network.
    topoSortTest2<MyNetType>("TopoSortTest:02:detect Cycles:StaticNet");
    topoSortTest2<MyNetType2>("TopoSortTest:01:detect Cycles:DynNet");
    topoSortTest2<MyNetType3>("TopoSortTest:01:detect Cycles:StaticFBNet");

}
