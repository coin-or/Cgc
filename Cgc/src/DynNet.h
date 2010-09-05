/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef DYNNET_H
#define DYNNET_H

#include <NodeId.h>
#include <map>
#include <set>
#include <list>
#include <functional>

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include <PLess.h>
#include <NodeIdMgr.h>

namespace Cgc
{

	// Forward template type declarations
	template < class NetType, class NodeInfo, class ArcInfo >
	class __ArcType;

	template < class NetType, class NodeInfo, class ArcInfo >
	class __NodeType;

	template < class NetType, class NodeInfo, class ArcInfo >
	class DynNetArcIterator;

	template < class NetType, class NodeInfo, class ArcInfo >
	class DynNetNodeIterator;

	template < class NetType, class NodeInfo, class ArcInfo>
	class ArcIterator;

	//MSVC OSTREAM / FRIEND BUG
#ifdef MSVCBUGS

#pragma warning(disable:4786)
#pragma warning(disable:4996)

	template < class NodeType, class NodeInfo, class ArcInfo>
	bool operator!=(const DynNetArcIterator<NodeType, NodeInfo, ArcInfo > &arcIt1,
		const DynNetArcIterator<NodeType, NodeInfo, ArcInfo > &arcIt2);

	template <class NodeType, class NodeInfo, class ArcInfo>
	bool operator==(const DynNetArcIterator<NodeType, NodeInfo, ArcInfo > &arcIt1,
		const DynNetArcIterator<NodeType, NodeInfo, ArcInfo > &arcIt2);

	template < class NetType, class NodeInfo, class ArcInfo >
	ostream &operator<<(ostream &os, const __NodeType<NetType, NodeInfo,ArcInfo> &n);
	template < class NetType, class NodeInfo, class ArcInfo >
	bool operator!=(const __NodeType<NetType, NodeInfo,ArcInfo> &n1, const __NodeType<NetType, NodeInfo,ArcInfo> &n2);
	template < class NetType, class NodeInfo, class ArcInfo >
	bool operator<(const __NodeType<NetType, NodeInfo,ArcInfo> &n1, const __NodeType<NetType, NodeInfo,ArcInfo> &n2);
	template < class NodeInfo, class ArcInfo >
	ostream &operator<<(ostream &os, const __ArcType<NodeInfo,ArcInfo> &n);
#endif

	template < class NodeType, class NodeInfo, class ArcInfo>
	bool operator!=(const DynNetNodeIterator<NodeType, NodeInfo, ArcInfo > &arcIt1,
		const DynNetNodeIterator<NodeType, NodeInfo, ArcInfo > &arcIt2);

	template <class NodeType, class NodeInfo, class ArcInfo>
	bool operator==(const DynNetNodeIterator<NodeType, NodeInfo, ArcInfo > &arcIt1,
		const DynNetNodeIterator<NodeType, NodeInfo, ArcInfo > &arcIt2);

	template < class NetType, class NodeInfo, class ArcInfo >
	bool operator!=(const ArcIterator<NetType,NodeInfo, ArcInfo > &arcIt1,
		const ArcIterator<NetType,NodeInfo, ArcInfo > &arcIt2);
	template < class NetType, class NodeInfo, class ArcInfo>
	bool operator==(const ArcIterator<NetType,NodeInfo, ArcInfo > &arcIt1,
		const ArcIterator<NetType,NodeInfo, ArcInfo > &arcIt2);


	template < class NodeInfo, class ArcInfo >
	class DynNet;

	// remaining declarations are as follows:
	// - Arc pointer declarations
	// - ArcIterator declaration, wraps ArcPnt (for operator * to work right)
	// - Node definition (uses bag of arc pointers, and ArcIterator)
	// - Node Bag declarations
	// - Arc Declaration (uses node bag iterator)
	// - Graph declaration
	// - functions that must have fully declared types to be complete

	/// @ingroup InternalUse
	template < class NetType, class NodeInfo, class ArcInfo>
	class DynNetArcTypes
	{
	public:
		typedef __ArcType<NetType, NodeInfo, ArcInfo > * __ArcTypePnt;
		typedef std::set<__ArcTypePnt ,PLess< __ArcTypePnt > > ArcBag;
	};
	/// @ingroup InternalUse
	template < class NetType,class NodeInfo, class ArcInfo >
	class ArcIterator
	{
	private:
		typedef typename DynNetArcTypes<NetType,NodeInfo, ArcInfo >::ArcBag MyArcBag;
		typedef typename MyArcBag::iterator MyArcBagIter;
		MyArcBagIter ait; 
		ArcIterator(const MyArcBagIter &iter):ait(iter)
		{}
	public:
		ArcIterator(){}
		ArcIterator(const ArcIterator &otherIter) 
			:ait(otherIter.ait) { }
		__ArcType<NetType,NodeInfo, ArcInfo > &operator*(){ return (*(*ait));}
		const __ArcType<NetType,NodeInfo, ArcInfo > &operator*()const { return (*(*ait));}
		int msvcCompareHack(const ArcIterator &otherIt)const
		{ 	  
			MyArcBagIter a = ait;
			MyArcBagIter b = otherIt.ait;
			return (a != b);
		}

		ArcIterator & operator=(const ArcIterator &otherIter)
		{ 
			ait = otherIter.ait; return *this; 
		}

		ArcIterator & operator++()
		{ 
			static ArcIterator tmp; 
			tmp.ait = ait;
			ait++; 
			return tmp;
		}

		ArcIterator & operator++(int)
		{ 
			ait++; 
			return *this;
		}

		friend class __NodeType<NetType, NodeInfo,ArcInfo>;
		friend class DynNet<NodeInfo,ArcInfo>;
	};

	/** @brief Node type for a dynamic network. DynNet
	@ingroup InternalUse
	@see DynNet
	*/
	template <class NetType, class NodeInfo, class ArcInfo >
	class __NodeType
	{
		typedef typename DynNetArcTypes<NetType, NodeInfo, ArcInfo >::ArcBag MyArcBag;
		typedef typename DynNetArcTypes<NetType, NodeInfo, ArcInfo >::__ArcTypePnt MyArcPnt;
		typedef typename MyArcBag::iterator MyArcBagIter;
		NodeInfo info;
		Cgc::NodeId nodeId;
		MyArcBag fwdArcs;
		MyArcBag backArcs;
		void setNodeId(const Cgc::NodeId &newId){ nodeId=newId;}
		__NodeType(const Cgc::NodeId &theId,const NodeInfo &ndVal)
			:info(ndVal),nodeId(theId){}
#ifdef MSVCBUGS
	public:
#endif
		Cgc::NodeId getNodeId()const{ return nodeId;}

	public:
		Cgc::NodeId getNodeIdHack()const{ return nodeId;}
		__NodeType(){}
		__NodeType(const NodeInfo &ndVal)
			:info(ndVal),nodeId(0){}
		__NodeType(const __NodeType &otherNode)
			: info(otherNode.info),
			nodeId(otherNode.nodeId),
			fwdArcs(otherNode.fwdArcs),
			backArcs(otherNode.backArcs)
		{}
		typedef ArcIterator<NetType, NodeInfo, ArcInfo > iterator;
		typedef ArcIterator<NetType, NodeInfo, ArcInfo > const_iterator;
		typedef ArcIterator<NetType, NodeInfo, ArcInfo > back_iterator;
		typedef ArcIterator<NetType, NodeInfo, ArcInfo > const_back_iterator;
	private:
		// remove the back-arc reference from the head node
		void back_erase(const __ArcType<NetType, NodeInfo,ArcInfo> *arcToErase);


		// remove the forward reference of a sequence of arcs...it also calls
		// back_erase to remove them from the back-list of the head-end
		void justErase(const iterator &only)
		{ 

			MyArcPnt mapnt= const_cast< MyArcPnt > (&(*only));
			MyArcBagIter mabi = fwdArcs.find(mapnt);
			fwdArcs.erase(mabi);
		}
		void erase(const iterator &first,const iterator &last)
		{
			for(iterator start=first;start!=last;start++)
			{
				__NodeType< NetType, NodeInfo, ArcInfo > &node = 
					const_cast< __NodeType< NetType ,NodeInfo, ArcInfo > & >((*(*start).head()));
				node.back_erase(&(*start));
			}
			for(iterator deleting=first;deleting!=last;deleting++)
			{
				delete &(*deleting);
			}
			fwdArcs.erase(first.ait,last.ait);
		}

		// allows me to use higher level iterator directly....
		void erase(const DynNetArcIterator<DynNet<NodeInfo,ArcInfo>,NodeInfo,ArcInfo>  &fst, const DynNetArcIterator<DynNet<NodeInfo,ArcInfo>,NodeInfo,ArcInfo>  &snd)
		{ erase(fst.arcIter,snd.arcIter); }

		typename __NodeType< NetType, NodeInfo, ArcInfo  >::iterator 
			insert(const __ArcType<NetType, NodeInfo, ArcInfo > &newArc);
		void back_insert(const __ArcType<NetType, NodeInfo,ArcInfo> *newArc);

		void disconnect()
		{
			iterator last = fwdArcs.end();
			for(iterator start=fwdArcs.begin();start!=last;start++)
			{
				__NodeType< NetType, NodeInfo, ArcInfo > &node = 
					const_cast< __NodeType< NetType, NodeInfo, ArcInfo > &>(*((*start).head()));
				node.back_erase(&(*start));
				delete &(*start);
			}
			iterator last2 = backArcs.end();
			for(iterator start = backArcs.begin(); start != last2;start++)
			{
				__NodeType< NetType, NodeInfo, ArcInfo > &node = 
					const_cast< __NodeType< NetType, NodeInfo, ArcInfo > &>(*((*start).tail()));
				node.justErase(start);
				delete &(*start);
			}
		}
	public:
		/// returns const reference to user data.
		const NodeInfo &operator*()const { return info;}
		/// returns reference to user data.
		NodeInfo &operator*(){ return info;}
		/// returns iterator to first arc leaving this node.
		iterator begin()
		{ return ArcIterator<NetType, NodeInfo, ArcInfo >(fwdArcs.begin());}
		/// returns const_iterator to first arc leaving this node.
		const_iterator begin()const
		{  
			__NodeType<NetType, NodeInfo,ArcInfo> *nonConstThis = const_cast<__NodeType<NetType, NodeInfo,ArcInfo> *>(this);
			MyArcBagIter iter = nonConstThis->fwdArcs.begin();
			return ArcIterator<NetType, NodeInfo,ArcInfo>(iter);
		}
		/// returns iterator to first backward arc entering this node.
		back_iterator back_begin()
		{ return ArcIterator<NetType, NodeInfo, ArcInfo >(backArcs.begin());}
		/// returns const_iterator to first backward arc entering this node.
		const_back_iterator back_begin()const
		{ return ArcIterator<NetType, NodeInfo, ArcInfo >(backArcs.begin());}

		/// returns iterator one past the last arc 
		iterator end()
		{ return ArcIterator<NetType, NodeInfo, ArcInfo >(fwdArcs.end()); }

		/// returns const_iterator one past the last arc 
		const_iterator end()const
		{ 
			__NodeType<NetType, NodeInfo,ArcInfo> *nonConstThis = const_cast<__NodeType<NetType, NodeInfo,ArcInfo> *>(this);
			MyArcBagIter iter = nonConstThis->fwdArcs.end();
			return ArcIterator<NetType, NodeInfo,ArcInfo>(iter);

		}

		/// returns iterator one past the last backward arc.
		back_iterator back_end()
		{ return ArcIterator<NetType, NodeInfo, ArcInfo >(backArcs.end()); }
		/// returns const_iterator one past the last backward arc.
		const_back_iterator back_end()const
		{ return ArcIterator<NetType, NodeInfo, ArcInfo >(backArcs.end()); }
		/// returns the number of backward arcs.
		size_t back_size()const{ return backArcs.size();}

		/// returns the number of forward arcs.
		size_t size()const{ return fwdArcs.size();}

	private:
		iterator findArc(MyArcPnt p)const
		{ 
			//      cout<<"has "<<fwdArcs.size()<<" connected arcs"<<endl;
			//iterator i = fwdArcs.find(p);
			//if(i==end())
			//cout<<"Didn't find it!"<<endl;
			//else
			//cout<<"found i = "<<(*i)<<endl;
			__NodeType<NetType, NodeInfo,ArcInfo> *nonConstThis = const_cast<__NodeType<NetType, NodeInfo,ArcInfo> *>(this);
			MyArcBagIter iter = nonConstThis->fwdArcs.find(p);
			return ArcIterator<NetType, NodeInfo,ArcInfo>(iter);
		}
	public:

		/** @brief comparisons for nodes.
		@ingroup PublicInterface
		*/
		friend bool operator==(const __NodeType<NetType, NodeInfo,ArcInfo> &nd1,const __NodeType<NetType, NodeInfo,ArcInfo> &nd2) 
		{ return nd1.nodeId == nd2.nodeId;}

		/** @brief comparisons for nodes.
		@ingroup PublicInterface
		*/
		friend bool operator!=(const __NodeType<NetType, NodeInfo,ArcInfo> &nd1,const __NodeType<NetType, NodeInfo,ArcInfo> &nd2) 
		{ return nd1.nodeId != nd2.nodeId;}

		/*
		friend bool operator<(const __NodeType<NodeInfo,ArcInfo> &nd1,const __NodeType<NodeInfo,ArcInfo> &nd2);
		*/

		void print(std::ostream &os)const;
		//friend std::ostream &operator<<(std::ostream &os, const __NodeType<NodeInfo, ArcInfo > &node);
		friend class __ArcType<NetType,NodeInfo, ArcInfo >;
		friend class DynNet<NodeInfo, ArcInfo >;
	};

	/// @ingroup InternalUse
	template <class NetType, class NodeInfo, class ArcInfo >
	class DynNetNodeTypes
	{
	public:
		typedef __NodeType<NetType, NodeInfo, ArcInfo > LocalNodeType;
		typedef std::map<NodeId, LocalNodeType > NodeBag;
	};

	/** @brief Arc Definition for DynNet arc types.
	@ingroup InternalUse
	*/
	template < class NetType,class NodeInfo ,class ArcInfo>
	class __ArcType
	{
		typedef DynNetNodeIterator<NetType,NodeInfo,ArcInfo> MyNodeBagIter;
		MyNodeBagIter tl;
		MyNodeBagIter hd;
		ArcInfo arcInfo;
	public:
		__ArcType(const MyNodeBagIter &tal,const ArcInfo &i,const MyNodeBagIter &hed)
			:tl(tal),hd(hed),arcInfo(i){}
		__ArcType(){}
		__ArcType(const __ArcType &otherArc)
			: tl(otherArc.tl),
			hd(otherArc.hd),
			arcInfo(otherArc.arcInfo){}
		/// returns an iterator to the node pointed to by this arc.
		MyNodeBagIter head()const{ return hd;}
		/// returns an iterator to the node pointed from with this arc.
		MyNodeBagIter tail()const{ return tl;}
		/// returns user data on this arc.
		ArcInfo &operator*(){ return arcInfo;}
		/// returns const user data on this arc.
		const ArcInfo &operator*()const{ return arcInfo;}
		/*
		inline friend bool operator<(const __ArcType<NodeInfo, ArcInfo > &arc1, 
		const __ArcType<NodeInfo, ArcInfo > &arc2) ;
		*/
		//friend std::ostream &operator<<(std::ostream &os, const __ArcType<NodeInfo, ArcInfo > &arc);

	};

	/// @ingroup InternalUse
	template < class NetType, class NodeInfo, class ArcInfo >
	class DynNetArcIterator
	{
		typedef typename DynNetArcTypes<NetType, NodeInfo, ArcInfo >::ArcBag MyArcBag;
		typedef ArcIterator<NetType, NodeInfo, ArcInfo > MyNodeArcIterator;

		NetType *net;
		DynNetNodeIterator<NetType,NodeInfo,ArcInfo> nodeIter;
		MyNodeArcIterator arcIter;
		bool atEnd()const
		{ return(nodeIter==net->end()); }
	public :
		//EQ
		bool compareHack(const DynNetArcIterator &otherIter)const
		{	
			if(nodeIter == otherIter.nodeIter)
			{
				if (nodeIter == net->end()) return true;
				if(arcIter == otherIter.arcIter)
				{
					return true;	
				}	
			}	
			return false;
		}
		//NE
		bool compareHack2(const DynNetArcIterator &otherIter)const
		{		
			if(atEnd() && otherIter.atEnd())
				return false;
			if(atEnd()||otherIter.atEnd())
				return true;
			if(nodeIter != otherIter.nodeIter)
				return true;
			if(arcIter != otherIter.arcIter)
				return true;
			return false;
		}
		DynNetArcIterator()
			:net(NULL)
		{
		}
		DynNetArcIterator(NetType &thenet)
			:net(&thenet),nodeIter(net->begin())
		{ if(net->begin()!=net->end()) arcIter=(*nodeIter).begin(); }

		DynNetArcIterator(NetType &thenet, const DynNetNodeIterator<NetType,NodeInfo,ArcInfo> &node)
			:net(&thenet),nodeIter(node)
		{ if(node != net->end()) arcIter=(*node).begin(); }

		DynNetArcIterator(NetType &thenet, 
			const DynNetNodeIterator<NetType,NodeInfo,ArcInfo> &node, 
			const MyNodeArcIterator &arc)
			:net(&thenet),nodeIter(node),arcIter(arc)
		{ }

		DynNetArcIterator(const DynNetNodeIterator<NetType,NodeInfo,ArcInfo> &node,
			const MyNodeArcIterator &arc)
			:nodeIter(node),arcIter(arc)
		{ }

		DynNetArcIterator(const DynNetArcIterator &otherArcIter)
			:net(otherArcIter.net),nodeIter(otherArcIter.nodeIter),
			arcIter(otherArcIter.arcIter){}

		DynNetArcIterator &operator=(const DynNetArcIterator &otherIter)
		{
			net=otherIter.net;
			if(otherIter.net!=NULL)
			{
				nodeIter=otherIter.nodeIter;
				if(otherIter.nodeIter != net->end())
				{
					arcIter=otherIter.arcIter;
				}
			}
			return *this;
		}
		DynNetArcIterator &operator=(const MyNodeArcIterator &nodeArcIter)
		{
			// this is kind of cheap.  It doesn't reassign the "net" reference
			// So it will fail if a MyNodeArcIterator from a different graph
			// is assigned into a DynNetArcIterator of a graph....
			nodeIter=(*nodeArcIter).tail();
			arcIter=nodeArcIter;
			return *this;
		}

		DynNetArcIterator operator++()
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
					{
						arcIter = (*nodeIter).begin();
					}
				}
			}
			return *this;
		}
		DynNetArcIterator operator++(int)
		{
			DynNetArcIterator tmp(*this);
			++(*this);
			return tmp;
		}
		const __ArcType<NetType, NodeInfo, ArcInfo > &operator*()const { return (*arcIter); }
		__ArcType<NetType, NodeInfo, ArcInfo > &operator*(){ return (*arcIter); }

		//friend class NetType;
		MyNodeArcIterator getArcIter(){ return arcIter;}
	};

	template < class NetType, class NodeInfo, class ArcInfo>
	class DynNetNodeIterator
	{
		typedef typename DynNetNodeTypes<NetType, NodeInfo, ArcInfo >::NodeBag MyNodeBag;
		typedef typename MyNodeBag::iterator MyNodeBagIter;
		NetType *net;
		MyNodeBagIter nodeIter;
		bool atEnd()const
		{ return(nodeIter==net->nodes.end()); }
	public :
		// EQ
		bool compareHack(const DynNetNodeIterator &otherIter)const
		{	
			if(nodeIter == otherIter.nodeIter)
			{
				return true;
			}	
			return false;
		}
		// NE
		bool compareHack2(const DynNetNodeIterator &otherIter)const
		{		
			if(atEnd() && otherIter.atEnd())
				return false;
			if(atEnd()||otherIter.atEnd())
				return true;
			if(nodeIter != otherIter.nodeIter)
				return true;
			return false;
		}
		DynNetNodeIterator()
			:net(NULL)
		{
		}
		DynNetNodeIterator(NetType &thenet)
			:net(&thenet),nodeIter(net->nodes.begin())
		{}

		DynNetNodeIterator(NetType &thenet, const MyNodeBagIter &node)
			:net(&thenet),nodeIter(node)
		{}

		DynNetNodeIterator &operator=(const DynNetNodeIterator &otherIter)
		{
			net=otherIter.net;
			nodeIter=otherIter.nodeIter;
			return *this;
		}

		__NodeType<NetType, NodeInfo,ArcInfo> &operator*()
		{
			return (*nodeIter).second;
		}

		const __NodeType<NetType, NodeInfo,ArcInfo> &operator*()const 
		{
			return(*nodeIter).second;
		}

		DynNetNodeIterator operator++()
		{
			assert(net != NULL);
			if(nodeIter!=net->nodes.end())
			{
				nodeIter++;
			}
			return *this;
		}
		DynNetNodeIterator operator++(int)
		{
			DynNetNodeIterator tmp(*this);
			++(*this);
			return tmp;
		}
		DynNetNodeIterator operator--()
		{
			if(nodeIter!=net->nodes.begin())
				nodeIter--;
			return *this;
		}
		DynNetNodeIterator operator--(int)
		{
			DynNetNodeIterator tmp(*this);
			--(*this);
			return tmp;
		}
	};
	/** @brief Dynamic Network -- allows altering the graph after construction.
	@ingroup Graphs

	*/


	template < class NodeInfo,class ArcInfo>
	class DynNet
	{
	private:
		typedef typename DynNetNodeTypes<DynNet, NodeInfo, ArcInfo >::NodeBag MyNodeBag;
		typedef DynNetNodeIterator<DynNet,NodeInfo,ArcInfo> MyNodeBagIter;
		friend class DynNetNodeIterator<DynNet,NodeInfo,ArcInfo>;
		Cgc::NodeIdMgr<DynNet> idMgr;
		MyNodeBag nodes;
		unsigned int hintNodes;
		unsigned int hintArcs;
		size_t arcSize;
	public:
		typedef __NodeType<DynNet,NodeInfo, ArcInfo > Node;
		typedef __ArcType<DynNet,NodeInfo, ArcInfo > Arc;
		typedef DynNetNodeIterator<DynNet,NodeInfo,ArcInfo> iterator;
		typedef DynNetNodeIterator<DynNet,NodeInfo,ArcInfo> const_iterator;
		typedef DynNetArcIterator<DynNet, NodeInfo, ArcInfo > const_arc_iterator;
		typedef DynNetArcIterator<DynNet, NodeInfo, ArcInfo  > arc_iterator;
		typedef NodeInfo node_data;
		typedef ArcInfo arc_data;

		DynNet(const int numNodes,const int numArcs)
			:idMgr(0),hintNodes(numNodes),hintArcs(numArcs),arcSize(0)
		{ }

		DynNet &operator=(const DynNet &otherNet)
		{exit(1); return *this;}
		DynNet(const DynNet &otherNet)
		{exit(1);}

		const_iterator begin()const
		{ 
			DynNet *nonConstThis = const_cast<DynNet *>(this);
			return DynNetNodeIterator<DynNet,NodeInfo,ArcInfo>((*nonConstThis),nonConstThis->nodes.begin());
			//return nodes.begin(); 
		}
		iterator begin()
		{
			return DynNetNodeIterator<DynNet,NodeInfo,ArcInfo>(*this,nodes.begin());
		}

		const_iterator end()const
		{ 
			DynNet *nonConstThis = const_cast<DynNet *>(this);
			return DynNetNodeIterator<DynNet,NodeInfo,ArcInfo>(*nonConstThis,nonConstThis->nodes.end()); 
		}
		iterator end()
		{
			return DynNetNodeIterator<DynNet,NodeInfo,ArcInfo>(*this,nodes.end());
		}
		void erase(const Cgc::NodeId &nodeId)
		{ 
			// go find the node..
			iterator nodeIt = find(nodeId);

			// have to maintain arcSize manually...the node deletes the arcs
			arcSize-=(*nodeIt).size();
			arcSize-=(*nodeIt).back_size();
			// remove the node's arcs
			Node &node = const_cast<Node &>(*nodeIt);
			node.disconnect();
			// remove the node
			nodes.erase(nodeId); 
		}
		void erase(const iterator &nodeIt)
		{ 
			// have to maintain arcSize manually...the node deletes the arcs
			arcSize-=(*nodeIt).size();
			arcSize-=(*nodeIt).back_size();

			// remove the node's arcs
			Node &node=const_cast<Node &>((*nodeIt));
			node.disconnect();
			// remove the node
			nodes.erase(nodes.find(getNodeId(nodeIt)));//getNodeId(*nodeIt)); 
		}
		void arc_erase(const arc_iterator &start, const arc_iterator &finish)
		{
			arc_iterator ait=start;
			// for each arc to remove
			while(ait!= finish)
			{
				// compute the next spot...
				arc_iterator next=ait;
				next++;
				typename Node::iterator last = ait.getArcIter();
				last++;
				// detach the arc from the head
				// NOTE: don't have to do back_erase...erase takes care of it
				Node &node = const_cast<Node &>((*(*ait).tail()));
				node.erase(ait.getArcIter(),last);

				arcSize--;
				ait=next;
			}
		}
		void erase(const iterator &first, const iterator &second)
		{ 
			std::list<NodeId> nodes;
			for(iterator nodeIt=first;nodeIt != second;nodeIt++)
			{
				nodes.push_back(getNodeId(nodeIt));
			}
			for(std::list<NodeId>::iterator nit= nodes.begin();
				nit!=  nodes.end();nit++)
			{
				iterator nodeIt = find((*nit));
				erase(nodeIt); 
			}
		}
		iterator insert(const NodeInfo &nd)
		{ 
			NodeId newId = idMgr.getNewId();
			std::pair<NodeId,__NodeType<DynNet,NodeInfo, ArcInfo > > myPair;
			myPair.first = newId;
			myPair.second = __NodeType<DynNet,NodeInfo, ArcInfo >(newId,nd);
			nodes.insert(myPair);
			return DynNetNodeIterator<DynNet,NodeInfo,ArcInfo>(*this, nodes.find(newId));
		}

		size_t size()const{ return nodes.size();}
		size_t arc_size()const{ return arcSize;}

		Cgc::NodeId getNodeId(const iterator &nodeIt)const{ return (*nodeIt).getNodeId();}

		const_arc_iterator arc_insert(iterator &tail, 
			const ArcInfo &info, 
			iterator &head)
		{
			arcSize++;
			Node &node=const_cast<Node &>(*tail);
			ArcIterator<DynNet,NodeInfo, ArcInfo > 
				arcIt( node.insert(__ArcType<DynNet,NodeInfo, ArcInfo >(tail,
				info,
				head)));
			//DynNet &net=*this;
			return arc_iterator(*this, tail, arcIt);
		}
		arc_iterator arc_begin()
		{ 
			if(size())
			{
				iterator nodeWithSize;
				for(nodeWithSize = begin()
					;((nodeWithSize!=end())
					&&((*nodeWithSize).size()==0))
					;nodeWithSize++)
				{}
				if(nodeWithSize==end())
					return arc_end();
				typename Node::iterator arcIt = (*nodeWithSize).begin();
				return arc_iterator(*this,nodeWithSize,arcIt);
			}
			return arc_iterator(*this,begin());
		}
		const_arc_iterator arc_begin()const
		{ 
			DynNet<NodeInfo, ArcInfo> *netpnt = 
				const_cast< DynNet<NodeInfo, ArcInfo> * >(this);
			if(size())
			{
				iterator nodeWithSize;
				for(nodeWithSize = netpnt->begin()
					;((nodeWithSize!=netpnt->end())
					&&((*nodeWithSize).size()==0))
					;nodeWithSize++)
				{}
				if(nodeWithSize==netpnt->end())
					return arc_end();
				typename Node::iterator arcIt = (*nodeWithSize).begin();
				return arc_iterator(*netpnt,nodeWithSize,arcIt);
			}
			return arc_iterator(*netpnt,netpnt->begin());
		}


		const_arc_iterator arc_end()const
		{ 
			DynNet<NodeInfo,ArcInfo> *netpnt= 
				const_cast< DynNet<NodeInfo,ArcInfo> * >(this);
			return const_arc_iterator(*netpnt,netpnt->end());
		}
		const_arc_iterator arc_find(const Cgc::NodeId &tailId,
			const Cgc::NodeId &headId)const
		{
			DynNet<NodeInfo,ArcInfo> *netpnt= 
				const_cast< DynNet<NodeInfo,ArcInfo> * >(this);
			iterator tail=find(tailId);
			/*
			if(tail!=end())
			cout<<"Found tail, id="<<getNodeId(tail)<<endl;
			else
			cout<<"NO TAIL NODE"<<endl;
			*/
			iterator head = find(headId);
			/*
			if(head!=end())
			cout<<"Found head, id="<<getNodeId(head)<<endl;
			else
			cout<<"NO HEAD NODE"<<endl;
			*/
			Arc anArc(tail,0,find(headId));
			//cout<<"Search Arc is "<<anArc<<endl;
			//cout<<"calling tail to find arc"<<endl;
			typename Node::iterator arcFound=(*tail).findArc(&anArc);
			if(arcFound==(*tail).end())
				return arc_end();
			else
				return arc_iterator(*netpnt,tail,arcFound);
		} 
		arc_iterator arc_find(const Cgc::NodeId &tailId, const Cgc::NodeId &headId)
		{
			DynNet<NodeInfo,ArcInfo> *netpnt= 
				const_cast< DynNet<NodeInfo,ArcInfo> * >(this);
			iterator tail=find(tailId);
			//if(tail!=end())
			//cout<<"Found tail, id="<<getNodeId(tail)<<endl;
			//else
			//cout<<"NO TAIL NODE"<<endl;
			//iterator head = find(headId);
			//if(head!=end())
			//cout<<"Found head, id="<<getNodeId(head)<<endl;
			//else
			//cout<<"NO HEAD NODE"<<endl;
			Arc anArc(tail,ArcInfo(),find(headId));
			//cout<<"Search Arc is "<<anArc<<endl;
			//cout<<"calling tail to find arc"<<endl;
			typename Node::iterator arcFound=(*tail).findArc(&anArc);
			if(arcFound==(*tail).end())
				return arc_end();
			else
				return arc_iterator(*netpnt,tail,arcFound);
		}

		const_iterator find(const NodeId &id)const
		{
			DynNet *nonConstThis = const_cast<DynNet *>(this);
			return DynNetNodeIterator<DynNet,NodeInfo,ArcInfo>((*nonConstThis), nonConstThis->nodes.find(id));
		}

		iterator find(const NodeId &id)
		{
			return( DynNetNodeIterator<DynNet,NodeInfo,ArcInfo>(*this,nodes.find(id) ));
		}

		iterator find(const NodeInfo &info)
		{
			for(iterator nodeIt = begin(); nodeIt != end();nodeIt++)
			{
				if((*(*nodeIt))==info)
					return nodeIt;
			}
			return end();
		}

		/*
		inline friend std::ostream &operator<<(std::ostream &os, 
		const DynNet<NodeInfo, ArcInfo > &net);
		*/

		friend class DynNetArcIterator<DynNet, NodeInfo, ArcInfo >;

		std::ostream &saveAdjacency(std::ostream &os)const
		{
			//const_iterator nodeIt;
			os<<"NetClass:Undir>"<<size()<<' '<<arc_size()<<std::endl;
			DynNet<NodeInfo,ArcInfo> *netpnt = 
				const_cast<DynNet<NodeInfo,ArcInfo> *>(this);
			for(iterator nodeIt = netpnt->begin();
				nodeIt != netpnt->end();nodeIt++)
			{
				os<<getNodeId(nodeIt)<<' '<<(*nodeIt).size()<<std::endl;
				for(typename Node::const_iterator arcIt=(*nodeIt).begin(); arcIt != (*nodeIt).end(); arcIt++)
					os<<getNodeId((*arcIt).head())<<' ';
				if((*nodeIt).size())
					os<<std::endl;
			}
			return os<<"Undir:Done"<<std::endl;
		}

		std::istream &loadAdjacency(std::istream &is)
		{
			char buffer[512];
			is.getline(buffer,512,'>');
			// check signature to see if a valid type
			if(strncmp(buffer,"NetClass:Undir",14))
			{
				std::cerr<<"Header wrong, can't read graph!"<<std::endl;
				return is;
			}
			// wipe out any existing graph
			erase(begin(),end());
			is>>hintNodes>>hintArcs;
			NodeId maxId=NodeId::getMinNodeId();

			ArcInfo defaultInfo;
			NodeId id;
			unsigned numAdj;
			iterator hdIter;
			iterator nodeIt;
			NodeInfo nd;

			for(unsigned nodeCnt = 0;nodeCnt<hintNodes;nodeCnt++)
			{
				is>>id>>numAdj;
				maxId=std::max(id,maxId);

				// may have been early insert item..so must check
				nodeIt = find(id);
				if(nodeIt==end())
				{
					std::pair<NodeId,__NodeType<DynNet,NodeInfo, ArcInfo > > newPair;
					newPair.first = id;
					newPair.second = __NodeType<DynNet,NodeInfo, ArcInfo >(id, nd); 
					nodes.insert(newPair);
					nodeIt = find(id);
				}

				for(;numAdj;numAdj--)
				{
					is>>id;// get head node
					hdIter = find(id);
					if(hdIter==end())
					{
						// do a pre-insert...need it now!
						std::pair<NodeId, __NodeType<DynNet,NodeInfo,ArcInfo> > newPair;
						newPair.first = id;
						newPair.second = __NodeType<DynNet,NodeInfo, ArcInfo >( id, nd); 
						nodes.insert(newPair);
						hdIter = find(id);
					}
					arc_insert(nodeIt,defaultInfo,hdIter);
				}
			}
			idMgr.setMaxId(maxId);
			is>>buffer;
			if(strncmp(buffer,"Undir:Done",10))
				std::cerr<<"Didn't get the last line right!"<<std::endl;
			return is;
		}
	};  

	//template < class NetType, class NodeInfo, class ArcInfo>
	//const __ArcType<NetType, NodeInfo, ArcInfo > &ArcIterator<NetType, NodeInfo, ArcInfo >::operator*()const
	//{ 
	//return (*(*ait)); 
	//}

	//template < class NetType, class NodeInfo, class ArcInfo>
	//__ArcType<NetType, NodeInfo,ArcInfo> &ArcIterator<NetType, NodeInfo,ArcInfo>::operator*()
	//{ 
	//return (*(*ait)); 
	//}

	template < class NetType, class NodeInfo, class ArcInfo>
	inline bool operator!=(const ArcIterator<NetType, NodeInfo,ArcInfo> &arcIt1,
		const ArcIterator<NetType, NodeInfo,ArcInfo> &arcIt2)
	{ 
		return arcIt1.msvcCompareHack(arcIt2) != 0;
	}

	template < class NetType, class NodeInfo, class ArcInfo>
	inline bool operator==(const ArcIterator<NetType, NodeInfo,ArcInfo> &arcIt1,
		const ArcIterator<NetType, NodeInfo,ArcInfo> &arcIt2)
	{ 
		return arcIt1.msvcCompareHack(arcIt2) == 0;
	}

	template <class NodeInfo, class ArcInfo>
	inline std::ostream &operator<<(std::ostream &os, 
		const DynNet<NodeInfo,ArcInfo> &net)
	{
		os<<"OutputNet["<<net.size()<<','<<net.arc_size()<<"]"<<std::endl;
		/*  */
		typename DynNet<NodeInfo,ArcInfo>::const_iterator last = net.end();
		for(typename DynNet<NodeInfo,ArcInfo>::const_iterator cNodeIt = 
			net.begin();
			cNodeIt != last;cNodeIt++)
		{ 
			os<<"Node "<<net.getNodeId(cNodeIt)<<" with "<<(*cNodeIt).size()<<" arcs out and "
				<<(*cNodeIt).back_size()<<" arcs in"<<std::endl;
		}
		for(typename DynNet<NodeInfo,ArcInfo>::const_arc_iterator cArcIt = 
			net.arc_begin();
			cArcIt != net.arc_end();cArcIt++)
		{ 
			os<<*cArcIt<<" 0---+ "<<net.getNodeId((*cArcIt).tail())<<" +---+ "
				<<net.getNodeId((*cArcIt).head())<<"+--->"<<std::endl; 
		}
		/*  */
		return os;
	}

	template <class NetType, class NodeInfo, class ArcInfo >
	typename __NodeType<NetType, NodeInfo,ArcInfo>::iterator
		__NodeType<NetType, NodeInfo,ArcInfo>::insert(const __ArcType<NetType, NodeInfo,ArcInfo> &newArc)
	{ 
		__ArcType<NetType, NodeInfo,ArcInfo> * lclArc= 
			new __ArcType<NetType, NodeInfo,ArcInfo>(newArc);

		MyArcBagIter ait = fwdArcs.insert(fwdArcs.end(),lclArc);
		__NodeType<NetType, NodeInfo,ArcInfo> &node 
			= const_cast< __NodeType<NetType, NodeInfo,ArcInfo> &>((*lclArc->head()));
		node.back_insert(lclArc);
		return ArcIterator<NetType, NodeInfo,ArcInfo>(ait);
	}

	template <class NetType, class NodeInfo, class ArcInfo >
	void __NodeType<NetType, NodeInfo,ArcInfo>::back_insert(const __ArcType<NetType, NodeInfo,ArcInfo> *newArc)
	{ 
#ifdef MSVCBUGS
		backArcs.insert(backArcs.end(),const_cast<__ArcType<NodeInfo,ArcInfo> *>(newArc));
#else
		__ArcType<NetType, NodeInfo,ArcInfo> *nonConst = 
			const_cast< __ArcType<NetType, NodeInfo,ArcInfo> *>(newArc);
		backArcs.insert(backArcs.end(),nonConst); 
#endif
	}

	template <class NetType, class NodeInfo, class ArcInfo >
	void __NodeType<NetType, NodeInfo,ArcInfo>::back_erase(const __ArcType<NetType, NodeInfo,
		ArcInfo> *arcToErase)

	{ 
		__ArcType<NetType, NodeInfo,ArcInfo> *nonConst=
			const_cast<__ArcType<NetType, NodeInfo,ArcInfo> *>(arcToErase);
		backArcs.erase(nonConst); 
	}

	template<class NetType, class NodeInfo, class ArcInfo>
	inline bool operator<(const __NodeType<NetType, NodeInfo,ArcInfo> &nd1,
		const __NodeType<NetType, NodeInfo,ArcInfo> &nd2)
	{ 
		return nd1.getNodeIdHack()< nd2.getNodeIdHack();
	}

	template <class NetType, class NodeInfo, class ArcInfo >
	inline bool operator<(const __ArcType<NetType, NodeInfo,ArcInfo> &arc1, 
		const __ArcType<NetType, NodeInfo,ArcInfo> &arc2) 
	{ 
		//cout<<"comparing arcs.."<<std::endl;
		//cout<<"arc1="<<&(*arc1.hd)<<"+"<<&(*arc1.tl)<<std::endl;
		//cout<<"arc2="<<&(*arc2.hd)<<"+"<<&(*arc2.tl)<<std::endl;
		//if(static_cast<int>(arc1.hd) < static_cast<int>(arc2.hd))
		if(&(*arc1.head()) <  &(*arc2.head()))
			return true;
		//if(static_cast<int>(arc1.hd) > static_cast<int>(arc2.hd))
		if(&(*arc1.head()) >  &(*arc2.head()))
			return false;
		//if(static_cast<int>(arc1.tl) < static_cast<int>(arc2.tl))
		if(&(*arc1.tail()) <  &(*arc2.tail()))
			return true;
		return false;
		/*
		THE FOLLOWING SECTION WAS CHANGED -> IT WAS BREAKING ARC_FIND
		//if(static_cast<int>(arc1.tl) > static_cast<int>(arc2.tl))
		//return false;
		//if(&(*arc1.tl) >  &(*arc2.tl)) -----> breaks arc_find
		//return *arc1 < *arc2; 
		*/
	}

	template < class NetType, class NodeInfo,class ArcInfo >
	inline bool operator!=(const DynNetArcIterator<NetType,NodeInfo,ArcInfo>  &rep1,
		const DynNetArcIterator<NetType,NodeInfo,ArcInfo>  &rep2)
	{
		return (rep1.compareHack2(rep2));
	}

	template <class NetType, class NodeInfo, class ArcInfo >
	inline bool operator==(const DynNetArcIterator<NetType,NodeInfo,ArcInfo> &rep1,
		const DynNetArcIterator<NetType,NodeInfo,ArcInfo> &rep2)
	{
		return (rep1.compareHack(rep2));
	}

	template < class NetType, class NodeInfo,class ArcInfo >
	inline bool operator!=(const DynNetNodeIterator<NetType,NodeInfo,ArcInfo>  &rep1,
		const DynNetNodeIterator<NetType,NodeInfo,ArcInfo>  &rep2)
	{
		return (rep1.compareHack2(rep2));
	}

	template <class NetType, class NodeInfo, class ArcInfo >
	inline bool operator==(const DynNetNodeIterator<NetType,NodeInfo,ArcInfo> &rep1,
		const DynNetNodeIterator<NetType,NodeInfo,ArcInfo> &rep2)
	{
		return (rep1.compareHack(rep2));
	}

	template < class NetType, class NodeInfo, class ArcInfo>
	std::ostream &operator<<(std::ostream &os, const __NodeType<NetType, NodeInfo, ArcInfo > &node)
	{ 
		return os<<*node;
	}

	template < class NetType, class NodeInfo, class ArcInfo>
	std::ostream &operator<<(std::ostream &os, const __ArcType<NetType, NodeInfo, ArcInfo > &arc)
	{ 
		return os<<*arc; 
	}

	template < class NetType, class NodeInfo, class ArcInfo >
	bool operator!=(const __NodeType<NetType, NodeInfo,ArcInfo> &n1, const __NodeType<NetType, NodeInfo,ArcInfo> &n2)
	{
		return (*n1) != (*n2);
	}
}

#endif
