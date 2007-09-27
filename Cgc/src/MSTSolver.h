/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef MSTSOLVER_H
#define MSTSOLVER_H

#include <Path.h>
#include <ArcCostComp.h>

#include <vector>
#include <iterator>
#include <algorithm>

namespace Cgc
{
	/** @brief Min cost spanning tree algorithm
	@ingroup Algorithms
	This solver uses disjoint set union in min arc cost order to form a
	minimum cost spanning forest.  If the graph is connected, it will be a
	min cost spanning tree.
	@li supplied Arc template data type must have a getCost() method.
	*/
	template < class NetType >
	class MSTSolver
	{
	public:
		typedef typename NetType::arc_iterator NetArcIter;
		typedef typename NetType::iterator NetIter;
		typedef std::vector<NetArcIter> ArcIndex;
		typedef typename std::vector<NetArcIter>::iterator ArcIndexIter;
		typedef typename NetType::Arc NetArc;
	private:
		ArcIndex arcCollection;
		ArcIndex solutionArcs;
		static void printArc(NetArc *arc){ std::cout<<"ARC:"<<*arc<<std::endl;}
	public:
		/** @brief extracts the solution from the solver.
		Gets a list of arcs in the min cost spanning tree.
		@note to get the total cost, iterate over this list summing up the
		individual arc costs.
		*/
		void getArcIndex(ArcIndex &arcs)
		{
			for(typename ArcIndex::iterator arcIdxIt= solutionArcs.begin();
				arcIdxIt!= solutionArcs.end(); arcIdxIt++)
				arcs.push_back(*arcIdxIt);
		}

		NodeId getAncestor(NetType &net, NodeId inputNodeId)
		{
			NodeId parentNodeId = (*(*(net.find(inputNodeId)))).getParent();
			while(inputNodeId!=parentNodeId)
			{
				inputNodeId = parentNodeId;
				parentNodeId = (*(*(net.find(inputNodeId)))).getParent();
			}
			return inputNodeId;
		}

		/** @brief Call this to find the min cost spanning forests.
		The answer will be stored in the solver.
		*/
		int solve(NetType &net)
		{
			NetIter endSpot2 = net.end();
			for(NetIter nodeIt = net.begin(); nodeIt != endSpot2 ; nodeIt++)
			{
				typename NetType::Node &n = 
					const_cast<typename NetType::Node &>(*nodeIt);
				(*n).setParent(net.getNodeId(nodeIt));
			}

			arcCollection.erase(arcCollection.begin(),arcCollection.end());
			solutionArcs.erase(solutionArcs.begin(),solutionArcs.end());
			//cout<<"s1"<<endl;

			NetArcIter endSpot= net.arc_end();

			for(NetArcIter arcIt = net.arc_begin(); 
				arcIt != endSpot; arcIt++)
				arcCollection.push_back(arcIt);

			//for_each(arcCollection.begin(),arcCollection.end(),printArc);

			Cgc::ArcCostComp<NetArcIter> arcCompObj;
			std::sort(arcCollection.begin(),arcCollection.end(),arcCompObj);
			//cout<<"s2"<<endl;

			unsigned int count=0;
			typename MSTSolver::ArcIndex::iterator arcIdx =  arcCollection.begin();
			bool done=false;
			//cout<<"s3"<<endl;
			while(!done)
			{
				//cout<<"s4"<<endl;
				if((count >= net.arc_size()) || solutionArcs.size() >= (net.size()-1))
					done = true;
				else
				{
					//cout<<"On interation count="<<count
					//<<" arcIdx points from Node labeled:"
					//<<*(*((*(*arcIdx)).tail()))
					//<<" to Node labeled:"
					//<<*(*((*(*arcIdx)).head()))
					//<<" with content "<<(*(*(*arcIdx)))<<endl;
					if(arcIdx == arcCollection.end())
					{
						done=true;
						//cout<<"Out of arcs!"<<endl;
					}
					else
					{
						NetIter hd = (*(*arcIdx)).head();
						NetIter tl = (*(*arcIdx)).tail();
						NodeId hdAncestor = getAncestor(net,net.getNodeId(hd));
						NodeId tlAncestor = getAncestor(net,net.getNodeId(tl));
						if(hdAncestor!=tlAncestor)
						{
							//cout<<"Disjoint set union on "<<*(*hd)<<" and "<<*(*tl)<<endl;
							// this could be faster.  Don't need lookup, but would
							// have to implement a min through a pointer
							//NetType::iterator newRoot;
							if(hdAncestor < tlAncestor)
							{
								typename NetType::Node &n=
									const_cast<typename NetType::Node &>
									( *(net.find(tlAncestor) ) );
								(*n).setParent(hdAncestor);

							}
							else
							{
								typename NetType::Node &n=
									const_cast<typename NetType::Node &>
									( *(net.find(hdAncestor) ) );
								(*n).setParent(tlAncestor);
							}
							solutionArcs.push_back(*arcIdx);
						}
					}
				}
				arcIdx++;
				count++;
			}
			//cout<<"Solver complete!...returning zero"<<endl;
			endSpot2 = net.end();
			return 0;
		}
	};

};


#endif
