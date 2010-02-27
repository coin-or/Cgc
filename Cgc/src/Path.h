/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef PATH_H
#define PATH_H

#include <vector>
#include <NodeId.h>
#include <algorithm>
namespace Cgc
{

	/** @brief Path used with shortest paths.
	@ingroup Algorithms
	This data type is used with several of the shortest path Algorithms to
	return solution paths.  It contains a set of node IDs and paths.
	A reverse() method is provided in case one wishes to iterate
	through the node IDs in reverse order.
	*/
	class Path: public std::vector < Cgc::NodeId >
	{
		int cost;
	public:
		/// Default constructor
		Path():cost(0){}
		/// Getter for cost component.  For public consumption.
		int getCost(void)const{ return cost;}
		/** Setter for the cost value, often called from algorithms.
		Not likely called publicly.
		*/
		void setCost(const int newCost){ cost=newCost;}
		/// reverses the order of the NodeIds in the collection.
		void reverse()
		{
			std::reverse(begin(),end());
		}
		inline friend std::ostream &operator<<(std::ostream &os, const Path &p);
	};

	/// convenient print for Paths.
	inline std::ostream &operator<<(std::ostream &os, const Path &p)
	{
		os<<"Cost:"<<p.getCost()<<" Ids:";
		for(Path::const_iterator pIt = p.begin(); pIt != p.end();pIt++)
			os<<(*pIt)<<' ';
		return os;
	}
}

#endif

