/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include "MSTData.h"

bool operator < (const MSTNodeLabel &label1, const MSTNodeLabel &label2)
{ return (label1.parent < label2.parent); }

bool operator != (const MSTNodeLabel &label1, const MSTNodeLabel &label2)
{ return (label1.parent != label2.parent); }

bool operator == (const MSTNodeLabel &label1, const MSTNodeLabel &label2)
{ return (label1.parent == label2.parent); }

std::ostream &operator<<(std::ostream &os, const MSTNodeLabel &label)
{ 
  return os<<"MSTParent:"<<label.parent; 
}

bool operator < (const MSTArcCost &cost1, const MSTArcCost &cost2)
{ return (cost1.cost < cost2.cost); }

bool operator == (const MSTArcCost &cost1, const MSTArcCost &cost2)
{ return (cost1.cost == cost2.cost); }

std::ostream &operator<<(std::ostream &os, const MSTArcCost &cost)
{ return os<<"MSTCost="<<cost.cost; }


