/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef ARCCOSTCOMP_H
#define ARCCOSTCOMP_H

#include <functional>

namespace Cgc
{
/** Utility object which compares arc iterators by their stored user supplied template "cost".
@ingroup InternalUse
    */
template <class T>
struct ArcCostComp : std::binary_function<T, T, bool> {
  bool operator()(const T& x, const T& y) const 
  {
      // x is an arc iterator, *x is a Net::Node::Arc, *(*x) is the user supplied Arc template parameter type.
      return ((*(*x)) < (*(*y)));
  }
};

};
#endif
