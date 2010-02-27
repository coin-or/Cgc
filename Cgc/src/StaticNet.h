/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef STATICNET_H
#define STATICNET_H

#include <vector>
#include <algorithm>
#include <NodeId.h>
#include <iostream>
#include <assert.h>

namespace Cgc
{
	/** @defgroup InternalUse Internal use only
	@brief these are not intended for public consumption.
	*/
	/** @defgroup PublicInterface Public interface
	@brief these are intended for public consumption.
	*/
	/** @defgroup Graphs Graph classes
	@brief These are the main graph implementation classes.
	@ingroup PublicInterface
	*/

	template <class NodeInfo, class ArcInfo>
	class __StaticArcType;

#ifdef MSVCBUGS
	/// @TODO can we remove these?  Are the bugs fixed?
	template <class NodeInfo, class ArcInfo>
	class __StaticNodeType;
	template <class NodeInfo, class ArcInfo>
	bool operator<(const __StaticNodeType<NodeInfo,ArcInfo> &node1,
		const __StaticNodeType<NodeInfo,ArcInfo> &node2);
	template <class NodeInfo, class ArcInfo>
	bool operator==(const __StaticNodeType<NodeInfo,ArcInfo>&node1, 
		const __StaticNodeType<NodeInfo,ArcInfo>&node2 );
	template <class NodeInfo, class ArcInfo>
	bool operator!=(const __StaticNodeType<NodeInfo,ArcInfo>&node1, 
		const __StaticNodeType<NodeInfo,ArcInfo>&node2 );
#endif

	/** Internal class storing a node type. 
	@ingroup InternalUse
	@see StaticNet::Node
	*/
	template <class NodeInfo, class ArcInfo>
	class __StaticNodeType
	{
		typedef __StaticArcType<NodeInfo,ArcInfo> Arc;
		NodeInfo info;
		Arc * base;
		int numArcs;
	public:
		/** @brief default constructor for a node
		@note: calls default constructor on NodeInfo parameter from user of the graph.
		*/
		__StaticNodeType():base(NULL),numArcs(0){}
		// constructor for a node which takes nodeInfo as an argument
		__StaticNodeType(const NodeInfo &keyInfo)
			:info(keyInfo), base(NULL),numArcs(0){}
		typedef Arc * iterator;
		typedef Arc * reverse_iterator;
		typedef Arc const * const_iterator;
		//typedef const Arc * const const_reverse_iterator;

		/// Returns the number of arcs emitting from this node.
		size_t size()const{ return numArcs;}

		/// returns an iterator to the first arc emitting from this node.
		iterator begin(){ return base; }
		/// returns a const_iterator to the first arc emitting from this node.
		const_iterator begin()const { return base; }
		/// returns an iterator one-past-the-end of the arcs emitting from this node.
		iterator end(){ return base + numArcs; }
		/// returns a const_iterator one-past-the-end of the arcs emitting from this node.
		const_iterator end()const { return base + numArcs; }
		/// returns a reverse iterator pointing at the last arc emitting from this node.
		reverse_iterator rbegin(){ return base+numArcs-1; }
		//const_reverse_iterator rbegin()const { return base+numArcs-1; }
		/// returns a reverse iterator pointing one before the first arc emitting from this node.
		reverse_iterator rend(){ return base-1; }
		//const_reverse_iterator rend(){ return base-1; }

		/// returns a non-const reference to the user supplied node data.
		NodeInfo &operator *(){ return info; }
		/// returns a const reference to the user supplied node data.
		const NodeInfo &operator *()const { return info; }

		/// allows insertion of an arc.  Insertion at the graph is preferred.
		iterator insert(iterator newArc)
		{ base = newArc - numArcs; numArcs++; return newArc; }

	};

	/** @brief Utility for comparing Nodes.
	@ingroup InternalUse
	*/
	template <class NodeInfo, class ArcInfo >
	bool operator==(const __StaticNodeType<NodeInfo,ArcInfo>&node1, 
		const __StaticNodeType<NodeInfo,ArcInfo>&node2 )
	{return (*node1 == *node2 ) ;}

	/** @brief Utility for comparing Nodes.
	@ingroup InternalUse
	*/
	template <class NodeInfo, class ArcInfo >
	bool operator!=(const __StaticNodeType<NodeInfo,ArcInfo>&node1, 
		const __StaticNodeType<NodeInfo,ArcInfo>&node2 )
	{return (*node1 != *node2 ) ;}


	/** @brief Utility typedefs to assist in graph declaration.  Required for some compiler versions.
	@ingroup InternalUse
	*/
	template <class NodeInfo, class ArcInfo>
	class TypeDefs3
	{
	public:
		typedef __StaticNodeType<NodeInfo,ArcInfo> LocalNodeType;
		typedef std::vector<LocalNodeType> NodeCollection;
		typedef typename NodeCollection::iterator NodeIterator;
		typedef typename NodeCollection::iterator ConstNodeIterator;
	};

	/** Internal class storing a arc type.  
	@ingroup InternalUse
	@see StaticNet::Arc
	*/
	template <class NodeInfo, class ArcInfo>
	class __StaticArcType
	{
		typedef typename TypeDefs3<NodeInfo,ArcInfo>::NodeIterator NodeIterator;
		typedef typename TypeDefs3<NodeInfo,ArcInfo>::ConstNodeIterator ConstNodeIterator;
		ArcInfo info;
		NodeIterator hd;
	public:
		/** @brief Default constructor
		@note uses default constructor of user supplied arcInfo data type.
		*/
		__StaticArcType(){ } 
		/** @brief Constructor taking arc info and head node.
		*/
		__StaticArcType(const ArcInfo &arcInfo,const NodeIterator &headNode)
			:info(arcInfo),hd(headNode){} 
		/// returns the node iterator pointed to by this arc.
		NodeIterator head(){ return hd;}
		/// returns a const iterator to the node pointed to by this arc.
		ConstNodeIterator head()const { return hd;}
		/// used to set the head node for this arc.
		void setHead(const NodeIterator &newHead) { hd =newHead;}
		/// returns a non-const reference to user data stored in the arc (ArcInfo)
		ArcInfo &operator *(){ return info; }
		/// returns a const reference to user data stored in the arc (ArcInfo)
		const ArcInfo &operator *()const { return info; }
		/// comapares using are info.
		friend inline bool operator<(const __StaticArcType &arc1, 
			const __StaticArcType &arc2)
		{ return *arc1 < *arc2 ;}
		friend inline bool operator!=(const __StaticArcType &arc1, 
			const __StaticArcType &arc2)
		{ return *arc1 != *arc2 ;}
		friend inline bool operator==(const __StaticArcType &arc1, 
			const __StaticArcType &arc2)
		{ return *arc1 == *arc2 ;}

	};

	/** @brief STatic Network representation.
	@ingroup Graphs
	A Static Net is a graph who's structure can't change.
	A Static Net is useful because of compact storage and speed of
	access to elements.  Since we know apriori that the graph won't
	change size post-load, we can pack the data in without using
	extraneous pointers.  In this implementation, I use vectors to do 
	the actual storage.  They are allocated to precisely the "right" 
	size.   Access to any given node (given a node id) is achieved in 
	constant time because of the vector behavior.  
	Iterating is constant time.

	*/
	template < class NodeInfo , class ArcInfo > 
	class StaticNet 
	{
	private:
		// ***data members***
		const int hintArcs;
		const int hintNodes;
		int currentNodeTop;
		int currentArcTop;

		// ***private member functions
	public:
		typedef __StaticNodeType<NodeInfo,ArcInfo> Node ;
		typedef __StaticArcType<NodeInfo,ArcInfo> Arc;

	private:
		typedef typename TypeDefs3<NodeInfo,ArcInfo>::NodeCollection NodeCollection;
		NodeCollection nodes;

	public:
		/// Typedefs for iterators
		typedef typename NodeCollection::iterator iterator;
		typedef typename NodeCollection::const_iterator const_iterator;
		typedef typename NodeCollection::reverse_iterator reverse_iterator;
		typedef typename NodeCollection::const_reverse_iterator const_reverse_iterator;
		typedef NodeInfo node_data;
		typedef ArcInfo arc_data;

	protected:

	private:
		Arc *arcs;

		Node *cacheLastInsert; // for simple error checking on arc insert
	public:

		/** Constructor with estimated number of nodes and arcs.
		expects the nodes and arcs to be correct.  Uses that to construct
		the raw vectors that will hold the data.
		*/
		StaticNet(const int estNodes, const int estArcs )
			:hintArcs(estArcs),hintNodes(estNodes),
			currentNodeTop(0),currentArcTop(0),
			nodes(estNodes),
			cacheLastInsert(NULL)
		{
			arcs = new Arc[estArcs];
		};

		typedef Arc        *arc_iterator;
		typedef Arc const  *const_arc_iterator;
		typedef Arc        *arc_reverse_iterator;
		//typedef typename ArcCollection::const_reverse_iterator const_arc_reverse_iterator;

		/// returns a StaticNet::iterator to the first node.
		iterator begin() 
		{ return nodes.begin(); }
		/// returns a StaticNet::const_iterator to the first node.
		const_iterator begin()const
		{ return nodes.begin(); }

		/// returns a StaticNet::iterator one past the last node.
		iterator end() 
		{ iterator it= begin();
		advance(it,currentNodeTop);
		return it;}
		/// returns a StaticNet::const_iterator one past the last node.
		const_iterator end()const
		{ const_iterator it= begin();
		it+=currentNodeTop;
		return it;}
		//{ return &nodes[currentNodeTop]; }

		/// returns a StaticNet::reverse_iterator to the last node
		reverse_iterator rbegin() 
		{ return nodes.rbegin()+(nodes.size()-currentNodeTop); }
		//const_reverse_iterator rbegin()const{ return nodes.rbegin(); }
		/// returns a StaticNet::reverse_iterator one before the first node.
		reverse_iterator rend() { return nodes.rend();}
		//const_reverse_iterator rend()const{ return nodes.rend(); }

		arc_iterator arc_begin() { return &(arcs[0]); }
		const_arc_iterator arc_begin()const{ return arcs; }
		arc_iterator arc_end()
		{ return &(arcs[currentArcTop]);}
		/*arc_iterator ai = arcs.begin();
		advance(ai,currentArcTop); return ai;}*/
		const_arc_iterator arc_end()const{ return arcs+currentArcTop; }

		arc_reverse_iterator arc_rbegin() { return arcs[currentArcTop-1]; }
		//const_arc_reverse_iterator arc_rbegin()const{ return arcs.rbegin(); }
		arc_reverse_iterator arc_rend() { return --(&(arcs[0])); }
		//const_arc_reverse_iterator arc_rend()const{ return arcs.rend(); }
		/// Node typedef fullfilling part of the graph contract.
		typedef Node NodeType;
		/// Arc typedef fullfilling part of the graph contract.
		typedef Arc ArcType;
		/** @brief inserts a node into the graph.  
		Returns an iterator to the node.  To get the nodeId, use getNodeId() and hand it this return value.
		@see getNodeId()
		*/
		iterator insert(const NodeInfo &nodeData)
		{ 
			assert(currentNodeTop < hintNodes);
			int thisNode=currentNodeTop;
			nodes[thisNode] = nodeData;
			iterator it = begin();
			advance(it,currentNodeTop);
			currentNodeTop++;
			return it;
		}

		/** @brief inserts an arc into the graph.  
		Returns an iterator to the arc.  
		@note this is the preferred method.
		*/
		arc_iterator arc_insert(iterator tailNode,
			const ArcInfo &info,
			iterator headNode)
		{
			assert(currentArcTop < hintArcs);
			if((cacheLastInsert!=NULL)&&(cacheLastInsert!=&(*tailNode)))
			{
				Node *pFirst = &(*begin());
				iterator it = begin();
				std::advance(it,(int)(cacheLastInsert-pFirst));
				if(getNodeId(it) > getNodeId(tailNode))
				{
					std::cerr<<"inserting arcs in improper order for static Network!"<<std::endl;
					std::cerr<<"Arc ignored"<<std::endl;
					return (*it).end();
				}
			}
			cacheLastInsert = &(*tailNode);
			arcs[currentArcTop]=Arc(info,headNode);
			(*tailNode).insert(&(arcs[currentArcTop]));
			currentArcTop++;
			return &(arcs[currentArcTop-1]);
		}

		/** @brief returns an iterator to a specific node by id.
		*/
		iterator operator[](const NodeId &id)
		{iterator it=begin();advance(it,id.getNodeId()); return it; }

		/** @brief returns a const_iterator to a specific node by id.
		*/
		const_iterator operator[](const NodeId &id)const
		{const_iterator it = begin();advance(it,id.getNodeId());return it;}

		/** @brief finds an iterator to a specific node by id.
		@note this is the preferred method.
		*/
		iterator find(const NodeId &id)
		{ iterator it = begin();advance(it,id.getNodeId());return it;}
		/** @brief finds an iterator to a specific node by id.
		@note this is the preferred method.
		*/
		const_iterator find(const NodeId &id)const
		{ const_iterator it = begin();advance(it,id.getNodeId());return it;}

		/** @brief finds an iterator to a specific node by NodeInfo.
		@note this may or may not be accurate depending on the user's implementation of NodeInfo.  In particular an operator< must be useful.
		*/
		iterator find(const NodeInfo &info)
		{return std::find(nodes.begin(),nodes.end(),Node(info));}
		/** @brief finds a const_iterator to a specific node by NodeInfo.
		@note this may or may not be accurate depending on the user's implementation of NodeInfo.  In particular an operator< must be useful.
		*/
		const_iterator find(const NodeInfo &info)const
		{return std::find(nodes.begin(),nodes.end(),Node(info));}

		/** @brief finds an arc iterator by tail node iterator and arc Info.
		@note may or may not be useful depending on the users' implementation of ArcInfo.
		*/
		arc_iterator arc_find(iterator  &nodeIter, const ArcInfo &info)
		{
			return std::find((*nodeIter).begin(),(*nodeIter).end(),Arc(info,begin()));
		}

		/** @brief finds an arc const_iterator by tail node iterator and ArcInfo.
		@note may or may not be useful depending on the users' implementation of ArcInfo.
		*/
		arc_iterator arc_find(const NodeId &id, const ArcInfo &info)
		{return std::find((*((*this)[id])).begin(),
		(*((*this)[id])).end(),Arc(info,begin()));}

		/** @brief finds an arc iterator by tail and head node iterators.
		*/
		const_arc_iterator arc_find(const_iterator &nodeTail,
			const_iterator &nodeHead)const
		{
			const typename Node::const_iterator e=(*nodeTail).end();
			for(typename Node::const_iterator aIt = (*nodeTail).begin() ; aIt != e; aIt++)
			{ if ((*aIt).head() == nodeHead) return (aIt);}
			return arc_end();
		}

		/** @brief finds an arc iterator by tail and head node iterators.
		*/
		arc_iterator arc_find(const NodeId &nodeTail,
			const NodeId &nodeHead)
		{
			const typename Node::iterator s=(*(*this)[nodeTail]).begin();
			const typename Node::iterator e=(*(*this)[nodeTail]).end();
			for(typename Node::iterator aIt = s ; aIt != e; aIt++)
			{ 
				if ((*aIt).head()==(*this)[nodeHead]) 
				{
					return aIt;
				}
			}
			return arc_end();
		}
		/** @brief finds an arc iterator by tail and head NodeId values.
		*/
		const_arc_iterator arc_find(const NodeId &nodeTail,
			const NodeId &nodeHead)const
		{
			const typename Node::const_iterator s=(*(*this)[nodeTail]).begin();
			const typename Node::const_iterator e=(*(*this)[nodeTail]).end();
			for(typename Node::const_iterator aIt = s ; aIt != e; aIt++)
			{ 
				if ((*aIt).head()==(*this)[nodeHead]) 
				{
					const_arc_iterator ai = arc_begin();
					advance(ai,aIt-&(*ai));
					return ai;
				}
			}
			return arc_end();
		}


		/** @brief finds an arc const_iterator by tail node iterator and ArcInfo.
		@note may or may not be useful depending on the users' implementation of ArcInfo.
		*/
		arc_iterator arc_find(const ArcInfo &info)
		{return std::find(arcs.begin(),arcs.end(),Arc(info,begin()));}

		/// returns the number of nodes in the graph.
		size_t size()const{ return currentNodeTop;}
		/// returns the number of arcs in the graph.
		size_t arc_size()const{ return currentArcTop;}

		/** looks up the nodeId given a nodeIterator.
		@note will return an invalid value on an end() iterator being sent in.
		*/
		NodeId getNodeId(const const_iterator &nodeIter)const
		{return NodeId(((int)(&(*nodeIter) -&(nodes[0]))));}
	};
	template <class NodeInfo, class ArcInfo>
	inline std::ostream &operator<<(std::ostream &os, const StaticNet<NodeInfo,ArcInfo> &net)
	{
		os<<"StaticNet["<<net.size()<<','<<net.arc_size()<<"]"<<std::endl;
		typename Cgc::StaticNet<NodeInfo,ArcInfo>::const_iterator last = net.end();
		for(typename Cgc::StaticNet<NodeInfo,ArcInfo>::const_iterator cNodeIt = 
			net.begin();cNodeIt != last;cNodeIt++)
		{ 
			os<<"Node "<<net.getNodeId(cNodeIt)<<" data:"<<*(*cNodeIt)<<" and "<<(*cNodeIt).size()<<" out arcs."<<std::endl;
		}
		for(typename StaticNet<NodeInfo,ArcInfo>::const_arc_iterator cArcIt = net.arc_begin();
			cArcIt != net.arc_end();cArcIt++)
		{ 
			os<<" o---+ "<<*(*cArcIt)
				<<net.getNodeId((*cArcIt).head())<<"+--->"<<std::endl; 
		}
		return os;
	}
}
#endif
