/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef LAYERSPSOLVER_H
#define LAYERSPSOLVER_H

#include <ArcPath.h>
#include <ArcHeadNodeCostComp.h>
#include <algorithm>
#include <assert.h>

#ifndef __LAYERSPMAXCOST
#define __LAYERSPMAXCOST INT_MAX
#endif

namespace Cgc
{
	/** @defgroup Algorithms Algorithms
	@ingroup PublicInterface
	@brief Algorithms which operate on graphs.
	All algorithms for traversal, solvers, etc. are contained in this group.
	*/
	/** @brief Shortest path solver which allows some arcs to be "turned off".
	This shortest path allows some arcs to be ignored during the call to the solver.  This is
	often used when graphs are reduced for either flow solvers or potentially during some sorts
	of column generation where the graph for the generation is reduced.
	It is a Dijkstra style heap based solver.
	@li Must have a bi-directional graph rep.  can go in reverse direction if "feasible".  
	Applies negative cost??  the getCost may handle that?
	@li heap contains arcs.  We cross these arcs to set nodes.  They become the contents of the back pointers.  
	@li User supplied Arc types must have operator< comparison
	@li User supplied node must have label information.
	@note: First call solve() which will solve source-to-all-reachable-nodes paths, then call getPath to get the min cost path to particular nodes.

	@ingroup Algorithms
	*/
	template < class NetType , class Foo >
	class LayerSPSolver
	{
		typedef typename NetType::Node NetNode;
		typedef typename NetType::Arc NetArc;
		typedef typename NetType::iterator NetIter;
		typedef typename NetNode::iterator NetNodeIter;
		typedef typename NetNode::back_iterator NetNodeBackIter;
		typedef typename NetType::iterator NodeIterator;
		/** Internal representation of an element in the heap.
		@ingroup InternalUse
		*/
		struct HeapContent
		{
			/// active arc 
			NetArc *arc; 
			/// perm label of pred + arc cost
			int cost; 
			/// true if arc used in fwd direction
			bool isFwd; 

			/// used for ordering heap
			bool operator<(const HeapContent &otherContent)const
			{ return cost < otherContent.cost; }
		};

		typedef std::vector < HeapContent > HeapType;
		HeapType layerHeap;

	public:
		/** @brief Solve a "layered" shortest path from source to all other reachable nodes.
		This performs the algorithm. Once completed, call getPath() to get paths out.
		*/
		int solve( NetType &net, 
			NodeIterator &source)
		{
			// clean out last use of heap
			layerHeap.erase(layerHeap.begin(),layerHeap.end());

			// reset labels to 'inifinity'
			for(NodeIterator nodeIter=net.begin();nodeIter != net.end();nodeIter++)
			{
				NetNode & nRef= const_cast<NetNode &>(*nodeIter);
				(*nRef).setLabel(__LAYERSPMAXCOST,net.getNodeId(nodeIter));
			}

			// initialize the source node with a zero label
			NetNode &nd = const_cast<NetNode &>(*source);
			(*(nd)).setLabel(0,0);

			// put each outbound arc onto the active heap.
			for( NetNodeIter arcIt = (*source).begin(); 
				arcIt!= (*source).end(); arcIt++)
			{
				HeapContent tempHeapItem;
				tempHeapItem.arc=&(*arcIt);
				tempHeapItem.isFwd=true;
				tempHeapItem.cost=(*(*arcIt)).getCost();
				layerHeap.push_back(tempHeapItem);
			}

			// while there are active arcs
			while(layerHeap.size())
			{
				// temps used
				HeapContent minItem;
				int nextValue;

				// search for a min cost improving arc in the heap
				do{
					std::less< HeapContent> heapItemComp;
					make_heap(layerHeap.begin(),layerHeap.end(), heapItemComp );
					pop_heap(layerHeap.begin(),layerHeap.end());

					// copy out the values from the heap
					minItem=layerHeap.back();
					layerHeap.pop_back();

					// if is a fwd arc, need the head label, else get tail label
					// If this arc won't improve the label, the label is permanent
					// and we should go to the next arc.
					if(minItem.isFwd)
						nextValue=(*(*(minItem.arc->head()))).getLabel();
					else
						nextValue=(*(*(minItem.arc->tail()))).getLabel();
					// stop when 1- heap empty, or 2- arc improves label
				}while(layerHeap.size()&&(minItem.cost >= nextValue));

				// either the cost is less, or the heap was exhausted.
				// if heap exhausted, done.  This tests cost.
				if(minItem.cost < nextValue)
				{
					// depending on direction this arc is used in, figure
					// out which node is predicessor and which is node to
					// update.
					typename NetType::iterator pred;
					typename NetType::iterator updateNode;
					if(minItem.isFwd)
					{
						updateNode = minItem.arc->head();
						pred = minItem.arc->tail();
					}
					else
					{
						updateNode = minItem.arc->tail();
						pred = minItem.arc->head();
					}
					NodeId predId = net.getNodeId(pred);
					NetNode &nd = const_cast<NetNode &>(*updateNode);
					(*nd).setLabel(minItem.cost,predId);
					//
					// put all outbound arcs that are valid 
					//
					// NOTE: probably could add an upper bound check if we had 
					// one. would speed up the solver....(dominate active arcs)
					//
					for(NetNodeBackIter bai = (*updateNode).back_begin();
						bai != (*updateNode).back_end();bai++)
					{
						HeapContent tempHeapItem;
						if((*(*bai)).backValid())
						{
							//get cost to end of this arc
							tempHeapItem.cost=(*(*updateNode)).getLabel()
								+ (*(*bai)).getCost();
							// currently, does this improve the back label?
							if(tempHeapItem.cost > 
								(*(*((*bai).tail()))).getLabel())
							{
								// finish assignment and push onto heap
								tempHeapItem.arc=&(*bai);
								tempHeapItem.isFwd=false;
								layerHeap.push_back(tempHeapItem);
								push_heap(layerHeap.begin(),layerHeap.end());
							}
							// else discard the arc
						}
						// else discard the arc
					}
					for(NetNodeIter fai = (*updateNode).begin();
						fai != (*updateNode).end();fai++)
					{
						HeapContent tempHeapItem;
						if((*(*fai)).valid())
						{
							//get cost to end of this arc
							tempHeapItem.cost=(*(*updateNode)).getLabel()
								+ (*(*fai)).getCost();
							// currently, does this improve the label?
							if(tempHeapItem.cost > (*(*((*fai).tail()))).getLabel())
							{
								// finish assignment and push onto heap
								tempHeapItem.arc=&(*fai);
								tempHeapItem.isFwd=true;
								layerHeap.push_back(tempHeapItem);
								push_heap(layerHeap.begin(),layerHeap.end());
							}
							// else discard the arc
						}
						// else discard the arc
					}
				}
				else // arc heap was empty -> DONE!
				{
					assert(layerHeap.size()==0);
					//std::cout<<"arc heap empty! DONE!"<<std::endl;
				}
			}
			return 0;
		};
		/** @brief gets paths out of the graph
		Call this after the call to solve() to get the paths to particular sinks.  
		The results are good until solve() is called again or the graph is destroyed.
		*/
		void getPath( const NetType &net, 
			const NodeIterator &sink, 
			ArcPath< NetArc * > &pathToFill)const
		{
			pathToFill.erase(pathToFill.begin(),pathToFill.end());
			typename NetType::const_iterator thisNode = sink;
			typename NetType::const_iterator prevNode = thisNode;
			NodeId thisId = net.getNodeId(thisNode);
			pathToFill.setCost((*(*thisNode)).getLabel());

			// next predecessor
			NodeId predId = (*(*thisNode)).getPred();
			prevNode = net.find(predId);
			// repeat until we are back at the source.
			while( predId != net.getNodeId(thisNode)) // conditions for at root?
			{
				NetNodeIter arcIt;
				for(arcIt = (*prevNode).begin();
					arcIt != (*prevNode).end();arcIt++)
				{
					if((*arcIt).head()==thisNode)
					{
						pathToFill.push_back(&(*arcIt));
						break;
					}
				}
				// if wasn't in the forward arcs, try the back arcs.
				if(arcIt==(*prevNode).end()&&(*arcIt).head()!=thisNode)
				{
					for(NetNodeBackIter backIt = (*prevNode).back_begin();
						backIt != (*prevNode).back_end();backIt++)
					{
						if((*backIt).tail()==thisNode)
						{
							pathToFill.push_back(&(*backIt));
							break;
						}
					}  
				}
				thisNode = prevNode;
				predId = (*(*thisNode)).getPred();
				prevNode = net.find(predId);
				thisId = predId;
			}
		}
	};
}
#endif
