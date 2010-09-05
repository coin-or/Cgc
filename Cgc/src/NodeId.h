/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef NODEID_H
#define NODEID_H
#include <limits.h>
#include <iostream>
#include <vector>
// some forward declarations.
#pragma warning(disable:4786)
#pragma warning(disable:4996)

namespace Cgc
{
	class NodeId;
	std::ostream &operator<<(std::ostream &os, const NodeId &id);
	std::istream &operator>>(std::istream &is, NodeId &id);
	bool operator<(const NodeId &id1, const NodeId &id2);
	bool operator>(const NodeId &id1, const NodeId &id2);
	bool operator==(const NodeId &id1, const NodeId &id2);
	bool operator!=(const NodeId &id1, const NodeId &id2);
	/** @brief Utility class to assist in uniquely identifying nodes.  
	This represents a unique identifier used to look up nodes.  The are assigned by the Graph type and are looked up 
	through the graph using node iterators.
	@note: given the current implementation, the graphs are limited to the USHRT_MAX many nodes.
	@ingroup PublicInterface
	*/
	class NodeId 
	{
	private:
		unsigned short nodeId;
	public:
		/// returns the maximum possible node id.
		static unsigned short getMaxNodeId(){ return USHRT_MAX;}
		/// returns the minimum possible node id. (zero)
		static unsigned short getMinNodeId(){ return 0;}
		/// returns an unsigned short representation of the node id.
		unsigned short getNodeId()const{ return nodeId;}
		/// constructs a nodeid from an unsigned short.  Defaults to the maximum.
		NodeId(const unsigned short &id=USHRT_MAX):nodeId(id){}
		/// sets the nodeId to the maximum value. 
		void setMaxNodeId(){ nodeId = USHRT_MAX;}
		/// sets the nodeId to the minimum value. 
		void setMinNodeId(){ nodeId = 0;}
		/// increments the node id by one (post-increment)
		NodeId operator++(int){ NodeId tmp(nodeId); ++nodeId; return tmp;}
		/// pre-increments the node id by one 
		NodeId &operator++(){ ++nodeId; return *this;}
		/// assignment operator.
		const NodeId &operator=(const NodeId &otherId)
		{ nodeId = otherId.nodeId; return *this;}
		/// Convenient print function std::cout<<NodeId::getMinNodeId()<<std::endl; for example
		friend std::ostream &operator<<(std::ostream &os, const NodeId &id);
		/// Convenient scan function for reading a properly formatted NodeId
		friend std::istream &operator>>(std::istream &is, NodeId &id);
		/// Comparison functions for nodeIds.
		friend bool operator<(const NodeId &id1, const NodeId &id2)
		{ return id1.nodeId < id2.nodeId;} 
		friend bool operator>(const NodeId &id1, const NodeId &id2)
		{ return id1.nodeId > id2.nodeId;}
		friend bool operator==(const NodeId &id1, const NodeId &id2)
		{ return id1.nodeId == id2.nodeId;}
		friend bool operator!=(const NodeId &id1, const NodeId &id2)
		{ return id1.nodeId != id2.nodeId;}
	};
	inline std::ostream &operator<<(std::ostream &os, const NodeId &id)
	{ return os<<'N'<<'^'<<id.nodeId; }

	inline std::istream &operator>>(std::istream &is, NodeId &id)
	{ 
		char ch;
		is>>ch;
		is>>ch;
		is>>id.nodeId;
		return is;
	}
} 
#endif
