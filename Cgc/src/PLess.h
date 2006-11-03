/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef PLESS_H
#define PLESS_H

#include <functional>

namespace Cgc
{
    /** @brief comparison object for pointers.  Compares what the pointer points too.
    @ingroup InternalUse
        */
    template <class T>
    struct PLess : std::binary_function<T, T, bool> {
        bool operator()(const T& x, const T& y) const { return *x < *y; }
    };
};
#endif
