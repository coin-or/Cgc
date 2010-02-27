/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef DFTRAVERSAL_H
#define DFTRAVERSAL_H

#include <stack>
namespace Cgc
{

	/** @brief Depth First Traversal iterator pattern.
	Supply the graph in the constructor
	@li use operator* to get the node iterator.
	@li use operator= to assign the BFTraversal to an arbitrary point in the graph.
	@li use ++ on the object to advance to the next location in the traversal.
	@note: If the graph has cycles, this will enter an infinite loop.
	@ingroup Traversals
	@see BFTraversal
	*/
	template<class GraphClass>
	class DFTraversal
	{
		typedef typename GraphClass::iterator NodeIt;
		typedef typename GraphClass::Node GrNode;
		typedef typename GrNode::iterator GrNodeIt;
		std::stack< NodeIt > myQueue;
		GraphClass &graphRef;
	public:
		/// @brief Constructor takes a Network.
		DFTraversal(GraphClass &gr):graphRef(gr)
		{ NodeIt nit = gr.begin(); myQueue.push(nit);}

		/// Initializes the DFTraversal to a particular iterator in the graph.  Resets the iteration.
		DFTraversal &operator=(const NodeIt &it)
		{ 
			while(myQueue.size())myQueue.pop(); 
			myQueue.push(it); 
			return *this;
		}

		/// Returns the GraphClass::iterator which is currently referred to.
		NodeIt operator *()
		{return myQueue.top();}

		/// Advances the Traversal.
		NodeIt operator ++(int)
		{ 
			if(myQueue.size())
			{
				NodeIt gci =  myQueue.top();
				myQueue.pop();
				for( GrNodeIt arcIter= (*gci).begin();
					arcIter != (*gci).end();arcIter++)
					myQueue.push((*arcIter).head());
				return gci;
			}
			return myQueue.top();
		}

		/// Comparison determining if one traversal object is identical in graph and location to the other.
		inline friend bool operator==(const DFTraversal &dft, 
			const NodeIt &it)
		{
			if(dft.myQueue.size())
				return (dft.myQueue.top()==it);
			if(it == dft.graphRef.end())
				return true;
			return false;
		}

#ifndef GNU_NE_BUG
		/** At one time G++ had a bug.  Don't know if it's still true.  
		@TODO research to see if g++ still has the bug requiring this hack.
		*/
		inline friend bool operator!=(const DFTraversal &dft, 
			const NodeIt &it)

		{
			if(dft.myQueue.size())
				return (dft.myQueue.top()!=it);
			if(it != dft.graphRef.end())
				return true;
			return false;
		}
#endif
	};
}
#endif
