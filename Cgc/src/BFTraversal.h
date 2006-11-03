/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef BFTRAVERSAL_H
#define BFTRAVERSAL_H

#include <list>
#include <queue>
namespace Cgc
{
    /** @defgroup Traversals Graph Traversal Utility Objects
        @ingroup PublicInterface
        @brief Graph Traversal Utility objects
        Use these for Breadth and Depth first traversals.
    */

    /** @brief Breadth First Traversal iterator pattern.
    Supply the graph in the constructor
    @li use operator* to get the node iterator.
    @li use operator= to assign the BFTraversal to an arbitrary point in the graph.
    @li use ++ on the object to advance to the next location in the traversal.
    @note: If the graph has cycles, this will enter an infinite loop.
    @ingroup Traversals
    @see DFTraversal
    */
    template<class GraphClass>
    class BFTraversal
    {
        typedef typename GraphClass::iterator NodeIt;
        typedef typename GraphClass::Node NetNode;
        typedef typename NetNode::iterator NetNodeIt;
        typedef typename GraphClass::arc_iterator ArcIt;
        std::queue< NodeIt > myQueue;
        GraphClass &graphRef;
    public:
        /// @brief Constructor takes a Network.
        BFTraversal(GraphClass &gr):graphRef(gr)
        { myQueue.push( gr.begin()); }
        /// Initializes the BFTraversal to a particular iterator in the graph.  Resets the iteration.
        BFTraversal &operator=(const NodeIt &it)
        { while(myQueue.size())myQueue.pop(); myQueue.push(it); return *this;}
        /// Returns the GraphClass::iterator which is currently referred to.
        NodeIt operator *()
        {return myQueue.front();}
        /// Advances the Traversal.
        NodeIt operator ++(int)
        { 
            if(myQueue.size())
            {
                NodeIt gci =  myQueue.front();
                myQueue.pop();
                for( NetNodeIt arcIter= (*gci).begin();
                    arcIter != (*gci).end();arcIter++)
                    myQueue.push((*arcIter).head());
                return gci;
            }
            return myQueue.front();
        }

        /// Comparison determining if one traversal object is identical in graph and location to the other.
        inline friend bool operator==(const BFTraversal &bft, 
            const NodeIt &it)
        {
            if(bft.myQueue.size())
                return (bft.myQueue.front()==it);
            if(it == bft.graphRef.end())
                return true;
            return false;
        }

#ifndef GNU_NE_BUG
        /** At one time G++ had a bug.  Don't know if it's still true.  
        @TODO research to see if g++ still has the bug requiring this hack.
        */
        inline friend bool operator!=(const BFTraversal &bft, 
            const NodeIt &it)

        {
            if(bft.myQueue.size())
                return (bft.myQueue.front()!=it);
            if(it != bft.graphRef.end())
                return true;
            return false;
        }
#endif
    };
}
#endif
