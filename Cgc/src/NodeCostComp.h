#ifndef NODECOSTCOMP_H
#define NODECOSTCOMP_H

#include <functional>

/** @brief comparison object for comparing nodes by their stored data type.
    @ingroup InternalUse
    "Node cost comparison" object.
*/
namespace Cgc
{
	template <class T>
	struct NodeCostComp : std::binary_function<T, T, bool> {
		bool operator()(const T& x, const T& y) const 
		{ 
			return ((*(*x)) > (*(*y))); 
		}
	};
}

#endif
