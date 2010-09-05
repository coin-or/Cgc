/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef STATICFBET_H
#define STATICFBET_H
#include <assert.h>
#include <vector>
#include <algorithm>
#include <NodeId.h>
#include <iostream>
/** @defgroup EXPERIMENTAL Experimental code, use at your own risk.
	@ingroup InternalUse
*/

namespace Cgc
{

	template < class NodeInfo, class ArcInfo >
	class SFBNArcType;

	template < class NodeInfo, class ArcInfo >
	class SFBNNodeType;

	template < class NetType, class NodeInfo, class ArcInfo >
	class SFBNetArcIterator;

	template < class NetType, class NodeInfo, class ArcInfo >
	class SFBNetConstArcIterator;

	template < class NodeInfo, class ArcInfo >
	class StaticFBNet;

	/// @ingroup EXPERIMENTAL
	template < class NodeInfo, class ArcInfo>
	class StaticFBNetArcTypes
	{
	public:
		typedef SFBNArcType<NodeInfo, ArcInfo >  __SFBNArcType;
		typedef __SFBNArcType * __SFBNArcTypePnt;
		typedef std::vector< __SFBNArcType> SFBNArcBag;
		typedef typename SFBNArcBag::iterator SFBNArcBagIterator;
		typedef std::vector< __SFBNArcTypePnt> SFBNBackArcBag;
		typedef typename SFBNBackArcBag::iterator SFBNBackArcBagIterator;
	};

	/// @ingroup EXPERIMENTAL
	template<class NodeInfo, class ArcInfo>
	class BackArcIterator
	{
		typedef typename StaticFBNetArcTypes< NodeInfo, 
			ArcInfo >::SFBNBackArcBag MyArcBag;
		typedef typename StaticFBNetArcTypes< NodeInfo, 
			ArcInfo >::SFBNBackArcBagIterator MyArcBagIter;

		MyArcBagIter ait; 

		BackArcIterator(const MyArcBagIter &iter):ait(iter)
		{}
	public:
		BackArcIterator(){}
		BackArcIterator(const BackArcIterator &otherIter) 
			:ait(otherIter.ait) { }

		SFBNArcType<NodeInfo,ArcInfo> &operator*(){ return (*(*ait));}
		const SFBNArcType<NodeInfo,ArcInfo> &operator*()const{ return (*(*ait));}
		int compareHack( const BackArcIterator<NodeInfo,ArcInfo> &arcIt2)const
		{
			if((ait==arcIt2.ait))
				return 0;
			return 1;
		}
		/*
		friend inline bool operator!=
		(const BackArcIterator<NodeInfo,ArcInfo> &arcIt1,
		const BackArcIterator<NodeInfo,ArcInfo> &arcIt2);

		friend inline bool operator==
		(const BackArcIterator<NodeInfo, ArcInfo > &arcIt1,
		const BackArcIterator<NodeInfo, ArcInfo > &arcIt2);
		*/

		BackArcIterator & operator=(const BackArcIterator &otherIter)
		{ ait = otherIter.ait; return *this; }

		BackArcIterator & operator++()
		{ 
			static BackArcIterator tmp; 
			tmp.ait = ait;
			ait++; 
			return tmp;
		}

		BackArcIterator & operator++(int)
		{ 
			ait++; 
			return *this;
		}
		friend class SFBNNodeType<NodeInfo,ArcInfo>;
	};

	/// @ingroup EXPERIMENTAL
	template < class NodeInfo, class ArcInfo >
	class SFBNNodeType
	{
		NodeInfo info;
		SFBNArcType<NodeInfo,ArcInfo> *base;

		typedef typename StaticFBNetArcTypes< NodeInfo, 
			ArcInfo >::SFBNBackArcBag SFBBackArcCollection;
		typedef typename StaticFBNetArcTypes< NodeInfo, 
			ArcInfo >::SFBNBackArcBagIterator SFBBackArcIterator;

		int numArcs;
		void setBegin(SFBNArcType<NodeInfo,ArcInfo> *spot){ base = spot; }
		SFBBackArcCollection backArcs;
	public:
		SFBNNodeType():base(NULL),numArcs(0){}
		SFBNNodeType(const NodeInfo &keyInfo)
			:info(keyInfo),base(NULL),numArcs(0){}
		SFBNNodeType(const SFBNNodeType &otherNode)
			:info(otherNode.info),base(otherNode.base),numArcs(otherNode.numArcs),
			backArcs(otherNode.backArcs){}
		SFBNNodeType &operator=(const SFBNNodeType &otherNode)
		{ 
			info=otherNode.info;
			base=otherNode.base;
			numArcs=otherNode.numArcs;
			SFBNNodeType &nonConstOther = const_cast<SFBNNodeType &>(otherNode);
			const BackArcIterator<NodeInfo,ArcInfo> &stopper(nonConstOther.back_end());
			for(BackArcIterator<NodeInfo,ArcInfo> backIt(nonConstOther.back_begin());
				backIt != stopper; backIt++)
				backArcs.push_back(*backIt.ait);
			return *this;
		}
		typedef SFBNArcType<NodeInfo,ArcInfo> *iterator;
		typedef SFBNArcType<NodeInfo,ArcInfo> *reverse_iterator;
		//typedef const SFBNArcType<NodeInfo,ArcInfo> * const const_iterator;
		typedef SFBNArcType<NodeInfo,ArcInfo> * const_iterator;
		typedef BackArcIterator<NodeInfo,ArcInfo> back_iterator;
		typedef BackArcIterator<NodeInfo,ArcInfo> const_back_iterator;


		size_t size()const{ return numArcs;}
		size_t back_size()const{ return backArcs.size();}

		iterator begin()
		{ 
			return base; 
		}
		const_iterator begin()const 
		{ 
			return base; 
		}
		iterator end()
		{ 
			return base + numArcs; 
		}
		const_iterator end()const { return base + numArcs; }

		back_iterator back_begin(){ return backArcs.begin(); }
		const_back_iterator back_begin()const 
		{ 
			SFBNNodeType *nonConstThis = const_cast<SFBNNodeType *>(this);
			const_back_iterator cbi;
			cbi = nonConstThis->backArcs.begin();
			return cbi;
		}
		back_iterator back_end()
		{ 
			back_iterator retVal = backArcs.end(); 
			return retVal;
		}
		const_back_iterator back_end()const 
		{ 
			const_back_iterator backEnd;
			SFBNNodeType *nonConstThis = const_cast<SFBNNodeType *>(this);
			backEnd = nonConstThis->backArcs.end();
			return backEnd;
			//return backArcs.end();
			//const_back_iterator retVal = nonConstthis->backArcs.end(); 
			//return retVal;
		}

		reverse_iterator rbegin(){ return base+numArcs-1; }
		//const_reverse_iterator rbegin()const { return base+numArcs-1; }
		reverse_iterator rend(){ return base-1; }
		//const_reverse_iterator rend(){ return base-1; }

		/*
		reverse_back_iterator back_rbegin()
		{ return backArcs.rbegin(); }
		//const_reverse_back_iterator back_rbegin(){ return backArcs.rbegin(); }

		reverse_back_iterator back_rend(){ return backArcs.rend(); }
		//const_reverse_back_iterator back_rend(){ return backArcs.rend(); }
		*/

		NodeInfo &operator *(){ return info; }
		const NodeInfo &operator *()const { return info; }

		iterator insert(SFBNArcType<NodeInfo,ArcInfo> *newArc)
		{ 
			assert(newArc);
			base = newArc - numArcs; numArcs++; 
			//... traverse to next node, insert backward arc
			(*newArc->head()).backArcs.push_back(newArc);
			return newArc; 
		}

		std::ostream & printNodeId(std::ostream &os, 
			const SFBNNodeType<NodeInfo,
			ArcInfo> *const nodeBase)const
		{ return os<<this-nodeBase; }

		bool operator==( const SFBNNodeType<NodeInfo,ArcInfo> &node2)
		{ return info==node2.info; }
		bool operator!=( const SFBNNodeType<NodeInfo,ArcInfo> &node2)
		{ return info!=node2.info; }
	};

	/// @ingroup EXPERIMENTAL
	template <class NodeInfo, class ArcInfo >
	class SFBNetNodeTypes
	{
	public:
		typedef SFBNNodeType<NodeInfo, ArcInfo > LocalNodeType;
		typedef std::vector<LocalNodeType> SFBNNodeBag;
		typedef typename SFBNNodeBag::iterator SFBNNodeBagIter;
		typedef typename SFBNNodeBag::const_iterator SFBNNodeBagConstIter;
	};


	template <class NodeInfo, class ArcInfo >
	bool operator==(const typename SFBNetNodeTypes<NodeInfo,ArcInfo>::SFBNNodeBag::iterator &iter1, 
		const typename SFBNetNodeTypes<NodeInfo,ArcInfo>::SFBNetNodeTypes::SFBNNodeBag::iterator &iter2)
	{
		return iter1 == iter2;
	}

	/// @ingroup EXPERIMENTAL
	template< class NodeInfo, class ArcInfo>
	class SFBNArcType
	{
		ArcInfo info;
		typedef typename SFBNetNodeTypes<NodeInfo,ArcInfo>::SFBNNodeBag MyNodeBag;
		typedef typename SFBNetNodeTypes<NodeInfo,ArcInfo>::SFBNNodeBagIter MyNodeBagIter;
		typedef typename SFBNetNodeTypes<NodeInfo,ArcInfo>::SFBNNodeBagConstIter 
			MyNodeBagConstIter;
		MyNodeBagIter hd;
		MyNodeBagIter tl;
	public:
		SFBNArcType(){ }
		SFBNArcType(const ArcInfo &arcInfo,
			const MyNodeBagIter &headNode,
			const MyNodeBagIter &tailNode)
			:info(arcInfo),hd(headNode),tl(tailNode){} 
		MyNodeBagIter head(){ return hd;}
		MyNodeBagIter tail(){ return tl;}
		MyNodeBagConstIter head()const { return hd;}
		MyNodeBagConstIter tail()const { return tl;}
		void setHead(const MyNodeBagIter &newHead) { hd =newHead;}
		void setTail(const MyNodeBagIter &newTail) { tl =newTail;}
		ArcInfo &operator *(){ return info; }
		const ArcInfo &operator *()const { return info; }
	};

	/// @ingroup EXPERIMENTAL
	template < class NetType, class NodeInfo, class ArcInfo >
	class SFBNetArcIterator
	{
		typedef typename SFBNetNodeTypes<NodeInfo, ArcInfo >::SFBNNodeBag MyNodeBag;
		typedef SFBNArcType<NodeInfo,ArcInfo> *MyArcBagIter;
	public:
		typedef typename MyNodeBag::iterator MyNodeBagIter;
	private:

		NetType *net;
		MyNodeBagIter nodeIter;
		MyArcBagIter arcIter;
		bool atEnd()const
		{ return(nodeIter==net->end()); }
	public :
		typedef SFBNetArcIterator<StaticFBNet<NodeInfo,ArcInfo>,
			NodeInfo,ArcInfo> arc_iterator;

		SFBNetArcIterator(NetType &thenet)
			:net(&thenet),nodeIter(net->begin())
		{ if(net->begin()!=net->end()) arcIter=(*nodeIter).begin(); }

		SFBNetArcIterator(NetType &thenet, const MyNodeBagIter &node)
			:net(&thenet),nodeIter(node)
		{ if(node != net->end()) arcIter=(*node).begin(); }

		SFBNetArcIterator(NetType &thenet, 
			const MyNodeBagIter &node, 
			const MyArcBagIter &arc)
			:net(&thenet),nodeIter(node),arcIter(const_cast<MyArcBagIter>(arc))
		{ }

		/*
		SFBNetArcIterator(const MyNodeBagIter &node,
		const MyArcBagIter &arc)
		:nodeIter(node),arcIter(arc)
		{ }
		*/

		SFBNetArcIterator(const SFBNetArcIterator &otherArcIter)
			:net(otherArcIter.net),nodeIter(otherArcIter.nodeIter),
			arcIter(otherArcIter.arcIter){}

		SFBNetArcIterator &operator=(const SFBNetArcIterator &otherIter)
		{
			net=otherIter.net;
			nodeIter=otherIter.nodeIter;
			arcIter=otherIter.arcIter;
			return *this;
		}

		SFBNetArcIterator &operator=(const MyArcBagIter &nodeArcIter)
		{
			// this is kind of cheap.  It doesn't reassign the "net" reference
			// So it will fail if a MyNodeArcIterator from a different graph
			// is assigned into a SFBNetArcIterator of a graph....
			nodeIter=(*nodeArcIter).tail();
			arcIter=nodeArcIter;
			return *this;
		}

		SFBNetArcIterator operator++()
		{
			// if past last node, are done, return "at end" 
			if(!atEnd())
			{
				if(arcIter!=(*nodeIter).end())
					arcIter++;
				while(!atEnd()&&arcIter==(*nodeIter).end())
				{
					nodeIter++;
					if(!atEnd())
						arcIter = (*nodeIter).begin();
				}
			}
			return *this;
		}
		SFBNetArcIterator operator++(int)
		{
			SFBNetArcIterator tmp(*this);
			++(*this);
			return tmp;
		}
		const SFBNArcType< NodeInfo, ArcInfo > &operator*()const 
		{ return (*arcIter); }
		SFBNArcType<NodeInfo, ArcInfo > &operator*(){ return (*arcIter); }

		int compareHack( const SFBNetArcIterator<NetType,NodeInfo,ArcInfo> 
			&rep2)const
		{
			if(nodeIter==rep2.nodeIter)
			{
				if((atEnd()&&rep2.atEnd())
					||(arcIter==rep2.arcIter))
				{
					return 0;
				}
			}
			return 1;
		}

		friend bool operator==(const SFBNetArcIterator<NetType,NodeInfo,ArcInfo> 
			&rep1,
			const SFBNetArcIterator<NetType,NodeInfo,ArcInfo> 
			&rep2)
		{
			return rep1.compareHack(rep2)==0;
		}
		friend bool operator!=(const SFBNetArcIterator<NetType,NodeInfo,ArcInfo>  
			&rep1,
			const SFBNetArcIterator<NetType,NodeInfo,ArcInfo>  
			&rep2)
		{
			return rep1.compareHack(rep2)!=0;
		}
		friend class SFBNetConstArcIterator<NetType,NodeInfo,ArcInfo>; 
	} ;
	/// @ingroup EXPERIMENTAL
	template < class NetType, class NodeInfo, class ArcInfo >
	class SFBNetConstArcIterator
	{
		typedef typename SFBNetNodeTypes<NodeInfo, ArcInfo >::SFBNNodeBag MyNodeBag;
		typedef SFBNArcType<NodeInfo,ArcInfo> *MyArcBagIter;
	public:
		typedef typename MyNodeBag::iterator MyNodeBagIter;
	private:

		NetType *net;
		MyNodeBagIter nodeIter;
		MyArcBagIter arcIter;
		bool atEnd()const
		{ return(nodeIter==net->end()); }
	public :
		typedef SFBNetConstArcIterator<StaticFBNet<NodeInfo,ArcInfo>,
			NodeInfo,ArcInfo> arc_iterator;
		SFBNetConstArcIterator()
			:net(NULL)
		{}
		SFBNetConstArcIterator(NetType &thenet)
			:net(&thenet),nodeIter(net->begin())
		{ 
			if(net->begin()!=net->end()) 
				arcIter=(*nodeIter).begin(); 
		}

		SFBNetConstArcIterator(const SFBNetArcIterator< NetType, NodeInfo, ArcInfo > &otherIter)
			:net(otherIter.net),nodeIter(otherIter.nodeIter), arcIter(otherIter.arcIter){}
		SFBNetConstArcIterator(NetType &thenet, const MyNodeBagIter &node)
			:net(&thenet),nodeIter(node)
		{ if(node != net->end()) arcIter=(*node).begin(); }

		SFBNetConstArcIterator(NetType &thenet, 
			const MyNodeBagIter &node, 
			const MyArcBagIter &arc)
			:net(&thenet),nodeIter(node),arcIter(const_cast<MyArcBagIter>(arc))
		{

		}

		/*
		SFBNetConstArcIterator(const MyNodeBagIter &node,
		const MyArcBagIter &arc)
		:nodeIter(node),arcIter(arc)
		{ }
		*/

		SFBNetConstArcIterator(const SFBNetConstArcIterator &otherArcIter)
			:net(otherArcIter.net),nodeIter(otherArcIter.nodeIter),
			arcIter(otherArcIter.arcIter){}

		SFBNetConstArcIterator &operator=(const SFBNetConstArcIterator &otherIter)
		{
			net=otherIter.net;
			nodeIter=otherIter.nodeIter;
			arcIter=otherIter.arcIter;
			return *this;
		}

		SFBNetConstArcIterator &operator=(const MyArcBagIter &nodeArcIter)
		{
			// this is kind of cheap.  It doesn't reassign the "net" reference
			// So it will fail if a MyNodeConstArcIterator from a different graph
			// is assigned into a SFBNetConstArcIterator of a graph....
			nodeIter=(*nodeArcIter).tail();
			arcIter=nodeArcIter;
			return *this;
		}

		SFBNetConstArcIterator operator++()
		{
			// if past last node, are done, return "at end" 
			if(!atEnd())
			{
				if(arcIter!=(*nodeIter).end())
					arcIter++;
				while(!atEnd()&&arcIter==(*nodeIter).end())
				{
					nodeIter++;
					if(!atEnd())
						arcIter = (*nodeIter).begin();
				}
			}
			return *this;
		}
		SFBNetConstArcIterator operator++(int)
		{
			SFBNetConstArcIterator tmp(*this);
			++(*this);
			return tmp;
		}
		const SFBNArcType< NodeInfo, ArcInfo > &operator*()const 
		{ return (*arcIter); }
		SFBNArcType<NodeInfo, ArcInfo > &operator*(){ return (*arcIter); }

		int compareHack( const SFBNetConstArcIterator<NetType,NodeInfo,ArcInfo> 
			&rep2)const
		{
			if(nodeIter==rep2.nodeIter)
			{

				if(atEnd() && rep2.atEnd())
					return 0;
				if(arcIter==rep2.arcIter)
				{
					return 0;
				}
			}
			return 1;
		}
		friend bool operator==(const SFBNetConstArcIterator<NetType,NodeInfo,ArcInfo> 
			&rep1,
			const SFBNetConstArcIterator<NetType,NodeInfo,ArcInfo> 
			&rep2)
		{
			return rep1.compareHack(rep2)==0;
		}
		friend bool operator!=(const SFBNetConstArcIterator<NetType,NodeInfo,ArcInfo>  
			&rep1,
			const SFBNetConstArcIterator<NetType,NodeInfo,ArcInfo>  
			&rep2)
		{
			return rep1.compareHack(rep2)!=0;
		}

	};
	/** Static undirected graph
	@ingroup Graphs
	@note experimental code.  
	*/
	template < class NodeInfo , class ArcInfo > 
	class StaticFBNet 
	{
	private:
		// ***data members***
		const int hintArcsPerNode;
		const int hintNodes;
		// ***private member functions
	protected:
	public:
		typedef SFBNNodeType<NodeInfo,ArcInfo> Node;

		/*
		friend bool operator<(const Node &node1, const Node &node2 )
		{return (*node1 < *node2 ) ;}
		friend bool operator ==(const Node &node1, const Node &node2 )
		{return (*node1 == *node2 ) ;}
		*/

	private:
		typedef typename SFBNetNodeTypes<NodeInfo,ArcInfo>::SFBNNodeBag NodeCollection;
		NodeCollection nodes;

	public:

		typedef typename NodeCollection::iterator iterator;
		typedef typename SFBNetNodeTypes<NodeInfo, ArcInfo>::SFBNNodeBag::const_iterator const_iterator;
		typedef ArcInfo arc_data;
		typedef NodeInfo node_data;
		//typedef typename NodeCollection::reverse_iterator reverse_iterator;
		//typedef NodeCollection::const_reverse_iterator const_reverse_iterator;

	public:
		typedef SFBNArcType<NodeInfo,ArcInfo> Arc;

		friend bool operator<(const Arc &arc1, const Arc &arc2)
		{ return *arc1 < *arc2 ;}
		friend bool operator==(const Arc &arc1, const Arc &arc2)
		{ return *arc1 == *arc2 ;}

		// ***public member functions***
	private:
		typedef typename StaticFBNetArcTypes< NodeInfo, 
			ArcInfo >::SFBNArcBag ArcCollection;
		//typedef vector < Arc > ArcCollection;
		ArcCollection arcs;
		Node *cacheLastInsert; // for simple error checking on arc insert
		int currentNumNodes;
		int currentNumArcs;

	public:

		StaticFBNet(const int estNodes, const int estArcs )
			:nodes(estNodes),arcs(estArcs),cacheLastInsert(NULL),
			currentNumNodes(0),currentNumArcs(0),hintArcsPerNode(0),hintNodes(estNodes)
		{}

		typedef SFBNetArcIterator<StaticFBNet<NodeInfo,ArcInfo>,
			NodeInfo,ArcInfo> arc_iterator;
		typedef SFBNetConstArcIterator<StaticFBNet<NodeInfo,ArcInfo>,
			NodeInfo,ArcInfo> const_arc_iterator;

		iterator begin() { return nodes.begin(); }
		const_iterator begin()const{ return nodes.begin(); }
		iterator end()
		{
			iterator it = nodes.begin();
			advance(it,currentNumNodes);
			return it;
		}
		const_iterator end()const
		{
			const_iterator it = nodes.begin();
			advance(it,currentNumNodes);
			return it;

		}

		/*
		reverse_iterator rbegin() 
		{ return nodes.rbegin()+(nodes.size()-currentNumNodes); }
		//const_reverse_iterator rbegin()const{ return nodes.rbegin(); }
		reverse_iterator rend() { return nodes.rend(); }
		//const_reverse_iterator rend()const{ return nodes.rend(); }
		*/

		arc_iterator arc_begin() 
		{
			return arc_iterator(*this);
		}
		const_arc_iterator arc_begin()const
		{ 
			StaticFBNet &nonConstThisRef = const_cast<StaticFBNet &>(*this);
			iterator node =nonConstThisRef.begin();
			while(node!=end()&&(*node).size()==0)
			{
				node++;
			}
			SFBNetConstArcIterator<StaticFBNet,NodeInfo,ArcInfo> retIt(nonConstThisRef,node);
			return retIt;
		}
		arc_iterator arc_end() 
		{
			return arc_iterator(*this,end());
			//return arc_iterator(const_cast<StaticFBNet &>(*this),end(),&(arcs[currentNumArcs]));
		}
		const_arc_iterator arc_end()const
		{
			StaticFBNet &nonConstThisRef = const_cast<StaticFBNet &>(*this);
			return const_arc_iterator(nonConstThisRef,
				nonConstThisRef.end(),
				NULL);
		}
		//const_cast<SFBNArcType<NodeInfo,ArcInfo> *>(
		//                                          &(arcs[currentNumArcs]))); }

		/*
		arc_reverse_iterator arc_rbegin() { return arcs.rbegin(); }
		//const_arc_reverse_iterator arc_rbegin()const{ return arcs.rbegin(); }
		arc_reverse_iterator arc_rend() { return arcs.begin(); }
		//const_arc_reverse_iterator arc_rend()const{ return arcs.rend(); }
		*/

		typedef Node NodeType;
		typedef Arc ArcType;

		iterator insert(const NodeInfo &nodeData)
		{ 
			Node newNode(nodeData);
			assert(currentNumNodes < hintNodes);
			nodes[currentNumNodes] = newNode;
			iterator it = begin();
			advance(it,currentNumNodes);
			currentNumNodes++;
			return it;
		}

		arc_iterator arc_insert(const iterator &tailNode,
			const ArcInfo &info,
			const iterator &headNode)
		{
			assert((unsigned)currentNumArcs<arcs.size());
			if((cacheLastInsert==NULL)||(cacheLastInsert<=&(*tailNode)))
			{
				cacheLastInsert = &(*tailNode);
				//arcs.insert(arcs.end(),Arc(info,headNode,tailNode));
				arcs[currentNumArcs]=Arc(info,headNode,tailNode);
				(*tailNode).insert(&(arcs[currentNumArcs]));
				currentNumArcs++;
				return arc_iterator(*this,tailNode,&(arcs[currentNumArcs-1]));
			}
			else
			{
				std::cerr<<"inserting arcs in improper order for static Network!"<<std::endl;
				std::cerr<<"Arc ignored"<<std::endl;
				return arc_end();
			}
		}

		iterator operator[](const Cgc::NodeId &id)
		{iterator it=begin();advance(it,id.getNodeId());return it;}

		const_iterator operator[](const Cgc::NodeId &id)const
		{const_iterator it=begin();advance(it,id.getNodeId());return it;}
		//{return const_cast<const_iterator>(&(nodes[id.getNodeId()]));}

		iterator find(const Cgc::NodeId &id)
		{ iterator it = begin();
		advance(it,id.getNodeId());
		return it; }
		const_iterator find(const Cgc::NodeId &id)const
		{ 
			const_iterator retVal = begin();
			std::advance(retVal,id.getNodeId());
			return retVal;
			//return &(nodes[id.getNodeId()]); 
		}

		iterator find(const NodeInfo &info)
		{
			return std::find(nodes.begin(),nodes.end(),Node(info));
		}

		arc_iterator arc_find(const_iterator  &nodeIter, const ArcInfo &info)
#ifdef GNU_LOST_CONST_HACK
		{return (arc_iterator) std::find((*nodeIter).begin(),
		(*nodeIter).end(),Arc(info,begin(),begin()));}
#else
		{return std::find((*nodeIter).begin(),
		(*nodeIter).end(),Arc(info,begin(),begin()));}
#endif

		arc_iterator arc_find(const Cgc::NodeId &id, const ArcInfo &info)
		{return std::find((*((*this)[id])).begin(),
		(*((*this)[id])).end(),Arc(info,begin(),begin()));}

		const_arc_iterator arc_find(const_iterator &nodeTail,
			const_iterator &nodeHead)const
		{
			const typename Node::const_iterator e=(*nodeTail).end();
			for(typename Node::const_iterator aIt = (*nodeTail).begin() ; aIt != e; aIt++)
			{ if ((*aIt).head() == nodeHead) return (aIt);}
			return arc_end();
		}

		const_arc_iterator arc_find(const Cgc::NodeId &nodeTail,
			const Cgc::NodeId &nodeHead)const
		{
			StaticFBNet &nonConstThisRef = const_cast<StaticFBNet &>((*this));

			iterator tail = nonConstThisRef[nodeTail];
			const_arc_iterator s(nonConstThisRef,tail,(*tail).begin());
			arc_iterator e(const_cast<StaticFBNet &>((*this)),tail,(*tail).end());
			for(const_arc_iterator arcIt= s; arcIt!=e;arcIt++)
			{
				if((*arcIt).head()==(*this)[nodeHead] )
					return arcIt;
			}
			return arc_end();
		}
		arc_iterator arc_find(const Cgc::NodeId &nodeTail,
			const Cgc::NodeId &nodeHead)
		{
			StaticFBNet &nonConstThisRef = const_cast<StaticFBNet &>((*this));

			iterator tail = nonConstThisRef[nodeTail];
			arc_iterator s(nonConstThisRef,tail,(*tail).begin());
			arc_iterator e(const_cast<StaticFBNet &>((*this)),tail,(*tail).end());
			for(arc_iterator arcIt= s; arcIt!=e;arcIt++)
			{
				if((*arcIt).head()==(*this)[nodeHead] )
					return arcIt;
			}
			return arc_end();
		}


		arc_iterator arc_find(const ArcInfo &info)
		{return std::find(arcs.begin(),arcs.end(),Arc(info,begin(),begin()));}

		size_t size()const{ return currentNumNodes;}
		size_t arc_size()const{ return currentNumArcs;}

		Cgc::NodeId getNodeId(const const_iterator &nodeIter)const
		{
			const const_iterator beg(begin());
			return Cgc::NodeId((unsigned short)std::distance(beg,nodeIter));
			//return Cgc::NodeId(((int)(&(*nodeIter) -&(nodes[0]))));
		}
		friend class SFBNetConstArcIterator< StaticFBNet<NodeInfo,ArcInfo>,
			NodeInfo, ArcInfo >;
		friend class SFBNetArcIterator< StaticFBNet<NodeInfo,ArcInfo>,
			NodeInfo, ArcInfo >;
	};

	/// @ingroup EXPERIMENTAL
	template < class NetType, class NodeInfo,class ArcInfo >
	inline bool operator!=(const SFBNetArcIterator<NetType,NodeInfo,ArcInfo>  
		&rep1,
		const SFBNetArcIterator<NetType,NodeInfo,ArcInfo>  
		&rep2)
	{
		return (rep1.compareHack(rep2));
	}

	/// @ingroup EXPERIMENTAL
	template <class NetType, class NodeInfo, class ArcInfo >
	inline bool operator==(const SFBNetArcIterator<NetType,NodeInfo,ArcInfo> &rep1,
		const SFBNetArcIterator<NetType,NodeInfo,ArcInfo> &rep2)
	{
		return rep1.compareHack(rep2)==0;
	}

	/// @ingroup EXPERIMENTAL
	template < class NodeInfo, class ArcInfo>
	inline bool operator!=(const BackArcIterator<NodeInfo,ArcInfo> &arcIt1,
		const BackArcIterator<NodeInfo,ArcInfo> &arcIt2)
	{ 
		return arcIt1.compareHack(arcIt2)!=0;
	}

	/// @ingroup EXPERIMENTAL
	template < class NodeInfo, class ArcInfo>
	inline bool operator==(const BackArcIterator<NodeInfo,ArcInfo> &arcIt1,
		const BackArcIterator<NodeInfo,ArcInfo> &arcIt2)
	{ 
		return arcIt1.compareHack(arcIt2)==0;
	}

	/// @ingroup EXPERIMENTAL
	template < class NodeInfo, class ArcInfo>
	std::ostream &operator<<(std::ostream &os, const SFBNNodeType<NodeInfo, ArcInfo > &node)
	{ 
		return os<<"SFBNNodeType:"<<*node; 
	}

	/// @ingroup EXPERIMENTAL
	template < class NodeInfo, class ArcInfo>
	std::ostream &operator<<(std::ostream &os, const SFBNArcType<NodeInfo, ArcInfo > &arc)
	{ 
		return os<<"SFBNArcType:"<<">"<<*arc<<'<'; 
	}

	template <class NodeInfo, class ArcInfo>
	inline std::ostream &operator<<(std::ostream &os, 
		const StaticFBNet<NodeInfo,ArcInfo> &net)
	{
		os<<"OutputNet["<<net.size()<<','<<net.arc_size()<<"]"<<std::endl;
		/*  */
		typename StaticFBNet<NodeInfo,ArcInfo>::const_iterator last = net.end();
		for(typename StaticFBNet<NodeInfo,ArcInfo>::const_iterator cNodeIt = 
			net.begin();
			cNodeIt != last;cNodeIt++)
		{ 
			os<<"Node "<<net.getNodeId(cNodeIt)<<" with "<<(*cNodeIt).size()<<" arcs out and "
				<<(*cNodeIt).back_size()<<" arcs in"<<std::endl;
		}
		for(typename StaticFBNet<NodeInfo,ArcInfo>::const_arc_iterator cArcIt = 
			net.arc_begin();
			cArcIt != net.arc_end();cArcIt++)
		{ 
			os<<*cArcIt<<" 0---+ "<<net.getNodeId((*cArcIt).tail())<<" +---+ "
				<<net.getNodeId((*cArcIt).head())<<"+--->"<<std::endl; 
		}
		/*  */
		return os;
	}
}
  
#endif
