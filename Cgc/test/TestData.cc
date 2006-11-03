/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/

#include "TestData.h"

std::ostream &operator<<(std::ostream &os, const NodeLabel &nodeI)
{ return os<<"nodeData.val="<<nodeI.val; }

std::ostream &operator<<(std::ostream &os, const ArcCost &cst)
{ return os<<"Cost="<<cst.val;}
