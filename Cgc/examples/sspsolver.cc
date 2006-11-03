/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <StaticNet.h>
#include <DijkSolver.h>
#include <iostream>
#include <set>
#include <algorithm>

using namespace Cgc;
/*
  This is a more comprehensive example which implements a well known method.
  See the reference:
  Network Flows, Ahuja, Magnanti, and Orlin, section 9.7, pg 321
 */
// Successive Shortest Path min cost flow 

class NodeData
{
public:
    NodeData(int eVal, int piVal)
    :e(eVal),pi(piVal),prevNode(0),label(0)
    { }
    NodeData()
    :e(0),pi(0),prevNode(0),label(0)
    { }
    int getLabel()const { return label;}
    void setLabel(int i,const NodeId &id)
    {
        label=i;
        prevNode=id;
    }
    int getE()const{ return e;}
    int getPi()const{ return pi;}
    void setE(int newE){ e=newE;}
    void setPi(int newPi){ pi = newPi;}
    NodeId getPred()const{ return prevNode;}
private:
    int e;
    int pi;
    NodeId prevNode;
    int label;
};

bool operator>(const NodeData &node1, const NodeData &node2)
{
    return node1.getLabel()>node2.getLabel();
}

std::ostream &operator<<(std::ostream &os, const NodeData &nd)
{
    os<<'('<<nd.getE()<<','<<nd.getPi()<<')';
    return os;
}

class ArcData
{
public:
    ArcData(int cVal, int rVal,bool isValid)
    :c(cVal),r(rVal),
     x(0),
     bValid(isValid)
    {
        if(bValid)
            cPi=cVal;
        else
            cPi=0;
    }
    ArcData()
    :c(0),cPi(0),r(0),
     x(0),
     bValid(true)
    { }
    int getC()const{ return c;}
    int getCPi()const{ return cPi;}
    int getR()const{ return r;}
    void setR(int newR){ r=newR;}
    void setCPi(int newCpi){ cPi=newCpi;}
    bool valid()const { return bValid;}
    void invalidate() { bValid=false;}
    void validate() { bValid=true;}
    int getCost()const { return cPi;}
    void incrX(int delta) { x+=delta;}
    int getX()const { return x;}
private:
    int c;
    int cPi;
    int r;
    int x;
    bool bValid;
};
std::ostream &operator<<(std::ostream &os, const ArcData &ad)
{
    os<<"o-(C="<<ad.getC()<<",CPi="<<ad.getCPi()<<",r="<<ad.getR()<<" x="<<ad.getX()<<")->";
    return os;
}

typedef StaticNet<NodeData,ArcData> NetType;
typedef StaticNet<NodeData,ArcData>::Node NodeType;
typedef StaticNet<NodeData,ArcData>::Arc ArcType;
// Note that because I have a typedef, we can coerce the compiler to print our way.
std::ostream & operator<<(std::ostream &os, const NetType &net)
{
    os<<"SSPNet|"<<net.size()<<"|:"<<std::endl;
    for(NetType::const_iterator nodeIt = net.begin();
        nodeIt!= net.end();++nodeIt)
        {
        os<<"Node["<<net.getNodeId(nodeIt)<<"]"<<*(*nodeIt)<<std::endl;
        for(NodeType::const_iterator arcIt = (*nodeIt).begin();
            arcIt!=(*nodeIt).end();arcIt++)
            {
            os<<"  ArcTo["<<net.getNodeId((*arcIt).head())<<"]"<<
                *(*arcIt);
            if((*(*arcIt)).valid())
                os<<"*"<<std::endl;
            else
                os<<"XXX"<<std::endl;
            }
        }
    return os;
}
void solve(NetType &net)
{
    int x = 0;
    typedef std::set<int> IntSet;
    IntSet E;
    IntSet D;
    for(unsigned cnt=0;cnt<net.size();cnt++)
        {
        if((*(*net.find(cnt))).getE()>0)
            E.insert(cnt);
        else
            if((*(*net.find(cnt))).getE()<0)
                D.insert(cnt);
        }
    while(E.size())
        {
        // pick a k
        int k = *(E.begin());
        // pick an l
        int l = *(D.begin());
        // solve all node shortest path.
        DijkSolver<NetType,NetType::iterator> solver;
        Path pathToFill;
        NetType::iterator s= net.find(k);
        //cout<<"Calling SP solver now"<<std::endl;
        solver.solve(net, s);
        // get the k-l shortest path.
        solver.getPath(net, net.find(NodeId(l)),pathToFill);
        pathToFill.reverse();
        std::cout<<"Path from "<<k<<" to "<<l<<" is "<<pathToFill<<std::endl;
        
        // update pi values for nodes. (pi = pi - d)
        for(NetType::iterator nodeIt = net.begin();nodeIt!=net.end(); nodeIt++)
            {
            NodeData &nd = (*(*nodeIt));
            int newPi = nd.getPi()-nd.getLabel();
            //cout<<"updating "<<net.getNodeId(nodeIt)<<" pi from "<<nd.getPi();
            //cout<<" to "<<newPi<<std::endl;
            nd.setPi(newPi);
            }
        // find min flow along k-l path for gam computation. (min r over path)
        int d = INT_MAX;
        for(unsigned cnt=0;cnt<pathToFill.size()-1;cnt++)
            {
            NetType::arc_iterator anArc=net.arc_find(pathToFill[cnt],
                                                           pathToFill[cnt+1]);
            int arcRVal = (*(*anArc)).getR();
            if(arcRVal<d)
                d=arcRVal;
            }
        //cout<<"Got min r from path to be "<<d<<std::endl;
        // compute gamma as min of source, sink, or d 
        NetType::iterator t = net.find(l);
        int sE = (*(*s)).getE() ;
        int tE = (*(*t)).getE() ;
        //cout<<"s->E="<<sE<<" -(t->E) = "<<-tE<<std::endl;
        int gam = std::min(std::min(sE,-tE), d);
        std::cout<<"DEBUG Got gam="<<gam<<std::endl;
        
        // for each in the k-l path, update R.
        for(unsigned cnt=0;cnt<pathToFill.size()-1;cnt++)
            {
            NetType::arc_iterator anArc=net.arc_find(pathToFill[cnt],
                                                     pathToFill[cnt+1]);
            NetType::arc_iterator itsReverseArc=net.arc_find(pathToFill[cnt+1],
                                                             pathToFill[cnt]);
            ArcData &ad = (*(*anArc));
            int arcRVal = ad.getR();
            ad.incrX(gam);
            ArcData &adReverse = (*(*itsReverseArc));
            int adReverseRVal = adReverse.getR();
            adReverse.incrX(-gam);
            
            ad.setR(arcRVal-gam);
            adReverse.setR(adReverseRVal+gam);
            // update G(x) by turning on/off arcs
            if(ad.getR()<=0)
                ad.invalidate();
            if(adReverse.getR()>0)
                adReverse.validate();
            // update x
            x+=ad.getC()*gam;
            }
        std::cout<<"*************(after augmenting graph)"<<std::endl;
        std::cout<<net<<std::endl;
        std::cout<<"*************"<<std::endl;
        std::cout<<"new X = "<<x<<std::endl;
        // update E
        (*(*s)).setE(sE-gam) ;
        (*(*t)).setE(tE+gam) ;
        // update E, & D
        if(sE-gam==0)
            {
            E.erase(E.find(net.getNodeId(s).getNodeId()));
            }
        if(sE+gam==0)
            {
            D.erase(E.find(net.getNodeId(t).getNodeId()));
            }
        // update and cPi
        // for each arc, compute cPi as c - pi(i)+pi(j);
        for(NetType::iterator nodeIt = net.begin();nodeIt!=net.end();nodeIt++)
            {
            int tailPi= (*(*nodeIt)).getPi();
            std::cout<<"From "<<net.getNodeId(nodeIt);
            for(NodeType::iterator ai=(*nodeIt).begin();
                ai!=(*nodeIt).end();ai++)
                {
                if((*(*ai)).valid())
                    {
                    int headPi = (*(*(*ai).head())).getPi();
                    ArcData &ad = (*(*ai));
                    int newCPi = ad.getC()-tailPi+headPi;
                    std::cout<<" to "<<net.getNodeId((*ai).head())<<" Tail pi is "<<tailPi<<" and head pi is "<<headPi<<" new CPi="<<newCPi<<std::endl;
                    
                    ad.setCPi(newCPi);
                    }
                }
            }
        std::cout<<"*************(PostIteration)"<<std::endl;
        std::cout<<net<<std::endl;
        std::cout<<"*************"<<std::endl;
        }
}

NetType *buildNet()
{
    NetType *n = new NetType(4,10);
    n->insert(NodeData(4,0));
    n->insert(NodeData(0,0));
    n->insert(NodeData(0,0));
    n->insert(NodeData(-4,0));
    //NetType::arc_iterator ni =
    NetType::iterator nodeZero=n->find(0);
    NetType::iterator nodeOne=n->find(1);
    NetType::iterator nodeTwo=n->find(2);
    NetType::iterator nodeThree=n->find(3);
    // arc one forward
    n->arc_insert(nodeZero,ArcData(2,4,true),nodeOne);
    //arc2forward
    n->arc_insert(nodeZero,ArcData(2,2,true),nodeTwo);
    // arc one reverse
    n->arc_insert(nodeOne,ArcData(-2,0,false),nodeZero);
    //arc3forward
    n->arc_insert(nodeOne,ArcData(1,2,true),nodeTwo);
    //arc4forward
    n->arc_insert(nodeOne,ArcData(3,3,true),nodeThree);
    //arc2reverse
    n->arc_insert(nodeTwo,ArcData(-2,0,false),nodeZero);
    //arc3reverse
    n->arc_insert(nodeTwo,ArcData(-1,0,false),nodeOne);
    //arc5forward
    n->arc_insert(nodeTwo,ArcData(1,5,true),nodeThree);
    //arc4reverse
    n->arc_insert(nodeThree,ArcData(-3,0,false),nodeOne);
    //arc5reverse
    n->arc_insert(nodeThree,ArcData(-1,0,false),nodeTwo);
    return n;
}

int main()
{
    NetType *n = buildNet();
    std::cout<<*n<<std::endl;    
    std::cout<<"Starting solver"<<std::endl;
    solve(*n);
    std::cout<<"Solver complete!"<<std::endl;
    return 0;
}

