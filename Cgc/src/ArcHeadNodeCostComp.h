/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef ARCHEADNODECOSTCOMP_H
#define ARCHEADNODECOSTCOMP_H

#include <functional>
namespace Cgc
{

	/** @brief cost of arcs based on head node cost.
	@ingroup InternalUse
	*/
	template <class T>
	struct ArcHeadCostComp : std::binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const 
		{ 
			return ( (*(*(x->head()))) > 
				(*(*(y->head()))) 
				);
		}
	};
}

#endif
