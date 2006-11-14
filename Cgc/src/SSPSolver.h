/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef SSPSOLVER_H
#define SSPSOLVER_H

#include <DijkSolver.h>
#include <iostream>
#include <set>

namespace Cgc
{

// Successive Shortest Path min cost flow -- Mangnanti Pg 321
// section 9.7
/** @brief  Supporting data structure for use with SSPSolver.
   @ingroup Algorithms
   @see SSPSolver.
 */
class MCFNodeData
{
public:
    MCFNodeData(int eVal, int piVal)
    :e(eVal),pi(piVal),prevNode(0),label(0)
    { }
    MCFNodeData()
    :e(0),pi(0),prevNode(0),label(0)
    { }
    int getLabel()const { return label;}
    void setLabel(int i,const Cgc::NodeId &id)
    {
        label=i;
        prevNode=id;
    }
    int getE()const{ return e;}
    int getPi()const{ return pi;}
    void setE(int newE){ e=newE;}
    void setPi(int newPi){ pi = newPi;}
    Cgc::NodeId getPred()const{ return prevNode;}
private:
    int e;
    int pi;
    Cgc::NodeId prevNode;
    int label;
} ;

    /** @brief comparison operator> for MCFNodeData
        @ingroup InternalUse
        @see SSPSolver.
    */
    inline bool operator>(const MCFNodeData &node1, const MCFNodeData &node2)
{
    return node1.getLabel()>node2.getLabel();
}

/** @brief Convenient print for MCFNodeData
    @ingroup InternalUse
   @see SSPSolver.
*/
inline std::ostream &operator<<(std::ostream &os, const MCFNodeData &nd)
{
    os<<'('<<nd.getE()<<','<<nd.getPi()<<')';
    return os;
}

/** @brief  Supporting data structure for use with SSPSolver.
   @ingroup Algorithms
   @see SSPSolver
 */
class MCFArcData
{
public:
    MCFArcData(int cVal, int rVal,bool isValid)
    :c(cVal),r(rVal),
     x(0),
     bValid(isValid)
    {
        if(bValid)
            cPi=cVal;
        else
            cPi=0;
    }
    MCFArcData()
    :c(0),cPi(0),r(0),
     x(0),
     bValid(true)
    { }
    int getC()const{ return c;}
    void setC(int newC){ c=newC;}
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
} ;

/** Convenient print
    @ingroup InternalUse
   @see SSPSolver
*/
inline std::ostream &operator<<(std::ostream &os, const MCFArcData &ad)
{
    os<<"o-(C="<<ad.getC()<<",CPi="<<ad.getCPi()<<",r="<<ad.getR()<<" x="<<ad.getX()<<")->";
    return os;
}

/** @brief  Supporting data structure for use with SSPSolver.
   @ingroup Algorithms
   @see SSPSolver.
   This contains the solution data for a flow solution.  
   Tail, Head, Flow, and arc iterator are contained in the inner
   FlowData struct.
   @see FlowData
   @see SSPSolver
 */
template <class NetType >
class SolutionType
{
public:
    typedef typename NetType::Node NodeType;
    typedef typename NetType::Arc ArcType;
    /// Container for flow data.
    typedef struct
    {
        typename NodeType::const_iterator arcIter;
        int flow;
        unsigned head;
        unsigned tail;
    } FlowData;
    const std::vector<FlowData> &getFlowData()const
    { return flowData;}
    void addFlowData(const FlowData &newData)
    {
        flowData.push_back(newData);
    }
    SolutionType(const NetType &net)
    :network(net)
    { }
    void clear()
    {
        flowData.clear();
    }
    void setX(int newX)
    {
        x=newX;
    }
    int getX()const
    {
        return x;
    }
                
private:
    std::vector<FlowData> flowData;
    int x;
    const NetType &network;
};

/** @brief Min Cost Flow Solver. Uses Successive shortest paths.
   @ingroup Algorithms
 */
template <class NetType >
class SSPSolver
{
    typedef typename NetType::Node NodeType;
    typedef typename NetType::Arc ArcType;
    int x;
public:
    typedef SolutionType<NetType> Solution;
    SSPSolver()
    :x(0)
    {}
    /// @ingroup InternalUse
    int getX()
    {
        return x;
    }
    /// this is the method to call when pulling the solution out.
    int getSolution(const NetType &net, Solution &sol)const
    {
        sol.clear();
        for(typename NetType::const_iterator nodeIt = net.begin();
            nodeIt!=net.end();nodeIt++)
            {
            for(typename NodeType::const_iterator ai=(*nodeIt).begin();
                ai!=(*nodeIt).end();ai++)
                {
                if((*(*ai)).getX()>0)
                    {
                    typename Solution::FlowData fp;
                    fp.arcIter=ai;
                    fp.flow=(*(*ai)).getX();
                    fp.tail=net.getNodeId(nodeIt).getNodeId();
                    fp.head=net.getNodeId((*ai).head()).getNodeId();
                    sol.addFlowData(fp);
                    }
                }
            }
        sol.setX(x);
        return x;
    }
    /// this is the method to call to solve the problem.
    bool solve(NetType &net)
    {
        int iterCnt=0;
        x = 0;
        typedef std::set<int> IntSet;
        IntSet E;
        IntSet D;
        for(unsigned cnt=0;cnt<net.size();cnt++)
            {
            typename NetType::iterator netIt = net.find(NodeId(cnt));
            ///@TODO why is the const_cast necessary?
            typename NetType::Node &node =
                const_cast<typename NetType::Node &>(*netIt);
            if((*node).getE()>0)
                E.insert(cnt);
            else
                if((*(*net.find(NodeId(cnt)))).getE()<0)
                    D.insert(cnt);
            }
        while(E.size())
            {
            ++iterCnt;
            if(iterCnt % 10000==0)
                {
                iterCnt=0;
                std::cout<<"MCFSolver: 10,000 iterations, E.size()="<<E.size()
                         <<" D.size()="<<D.size()<<std::endl;
                }
            // pick a k
            int k = *(E.begin());
            // pick an l
            int l = *(D.begin());
            // solve all node shortest path.
            DijkSolver<NetType, typename NetType::iterator> solver;
            Cgc::Path pathToFill;
            typename NetType::iterator s= net.find(NodeId(k));
            solver.solve(net, s);
            // get the k-l shortest path.
            solver.getPath(net, net.find(Cgc::NodeId(l)),pathToFill);
            if(pathToFill.size()==0)
                {
                std::cerr<<"Didn't find a path between "<<k<<" and "<<l<<".  aborting"<<std::endl;
                return false;
                }
            pathToFill.reverse();
        
            // update pi values for nodes. (pi = pi - d)
            for(typename NetType::iterator nodeIt = net.begin();
                nodeIt!=net.end(); nodeIt++)
                {
                // @TODO this shouldn't be necessary to const_cast this.
                // StaticFBNet failure.
                typename NetType::Node &nodeRef=const_cast<typename NetType::Node &>(*nodeIt);
                MCFNodeData &nd = (*nodeRef);
                int newPi = nd.getPi()-nd.getLabel();
                nd.setPi(newPi);
                }
            // find min flow along k-l path for gam computation. (min r over path)
            int d = INT_MAX;
            for(unsigned cnt=0;cnt<pathToFill.size()-1;cnt++)
                {
                typename NetType::arc_iterator anArc=net.arc_find(pathToFill[cnt],
                                                         pathToFill[cnt+1]);
                int arcRVal = (*(*anArc)).getR();
                if(arcRVal<d)
                    d=arcRVal;
                }
            // compute gamma as min of source, sink, or d 
            typename NetType::iterator t = net.find(NodeId(l));
            // @TODO the following const_casts should not be necessary (gcc3.x)
            typename NetType::Node &nS = const_cast<typename NetType::Node &>(*s);
            typename NetType::Node &nT = const_cast<typename NetType::Node &>(*t);
            //int sE = (*(*s)).getE() ;
            int sE = (*(nS)).getE() ;
            int tE = (*(nT)).getE() ;
            int gam = std::min(std::min(sE,-tE), d);
            //cout<<"Augmenting path by gamma="<<gam<<endl;
            //cout<<"s="<<(*(*s)).getE()<<endl;
            //cout<<"t="<<-(*(*t)).getE()<<endl;
        
            // for each in the k-l path, update R.
            for(unsigned cnt=0;cnt<pathToFill.size()-1;cnt++)
                {
                typename NetType::arc_iterator anArc=net.arc_find(pathToFill[cnt],
                                                         pathToFill[cnt+1]);
                typename NetType::arc_iterator itsReverseArc=net.arc_find(pathToFill[cnt+1],
                                                                 pathToFill[cnt]);
                assert(itsReverseArc!=net.arc_end()); // this solver requires both dirs.
                MCFArcData &ad = (*(*anArc));
                int arcRVal = ad.getR();
                ad.incrX(gam);
                MCFArcData &adReverse = (*(*itsReverseArc));
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
            // update E
            typename NetType::Node &nS2 = const_cast<typename NetType::Node &>(*s);
            typename NetType::Node &nT2 = const_cast<typename NetType::Node &>(*t);
            (*(nS2)).setE(sE-gam) ;
            (*(nT2)).setE(tE+gam) ;
            // update E, & D
            if(sE-gam==0)
                {
                E.erase(E.find(net.getNodeId(s).getNodeId()));
                }
            if(tE+gam==0)
                {
                D.erase(D.find(net.getNodeId(t).getNodeId()));
                }
            // update and cPi
            // for each arc, compute cPi as c - pi(i)+pi(j);
            for(typename NetType::iterator nodeIt = net.begin();
                nodeIt!=net.end();nodeIt++)
                {
                /// @TODO figure out why the const cast is necessary... gcc3.x
                    typename NetType::Node &node = const_cast<typename NetType::Node &>(*nodeIt);
                    //typename NetType::Node &node = (*nodeIt);
                int tailPi= (*node).getPi();
                for(typename NodeType::iterator ai=(*nodeIt).begin();
                    ai!=(*nodeIt).end();ai++)
                    {
                    if((*(*ai)).valid())
                        {
                        int headPi = (*(*(*ai).head())).getPi();
                        MCFArcData &ad = (*(*ai));
                        int newCPi = ad.getC()-tailPi+headPi;
                    
                        ad.setCPi(newCPi);
                        }
                    }
                }
            }
        return true;
    }
};
};

#endif
