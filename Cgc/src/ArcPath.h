/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef ARCPATH_H
#define ARCPATH_H

#include <vector>

namespace Cgc
{

    /** @brief a path based solely from arcs rather than nodes.
        @ingroup InternalUse
        @see LayerSPSolver
    */
template <class ArcIterator>
class ArcPath: public std::vector < ArcIterator >
{
	int cost;
public:
	ArcPath():cost(0){}
	int getCost(void)const{ return cost;}
	void setCost(const int newCost){ cost=newCost;}
};

template < class ArcIterator >
inline std::ostream &operator<<(std::ostream &os, const ArcPath< ArcIterator > &p)
{
	os<<"Cost:"<<p.getCost()<<" Arcs:";
	for(typename ArcPath<ArcIterator>::const_iterator pIt = p.begin(); 
	    pIt != p.end();pIt++)
		os<<(*pIt)<<' ';
	return os;
}
};

#endif
