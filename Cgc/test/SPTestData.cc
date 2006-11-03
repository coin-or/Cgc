/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include "SPTestData.h"

bool operator > (const SPNodeLabel &label1, const SPNodeLabel &label2)
{ return ( label1.label > label2.label); }

bool operator < (const SPNodeLabel &label1, const SPNodeLabel &label2)
{ return ( label1.label < label2.label); }

bool operator == (const SPNodeLabel &label1, const SPNodeLabel &label2)
{ return (label1.label == label2.label); }

bool operator != (const SPNodeLabel &label1, const SPNodeLabel &label2)
{ return (label1.label != label2.label); }

std::ostream &operator<<(std::ostream &os, const SPNodeLabel &label)
{ return os<<"Custom Label = "<<label.label; }

bool operator < (const SPArcCost &cost1, const SPArcCost &cost2)
{ return (cost1.cost < cost2.cost); }

bool operator == (const SPArcCost &cost1, const SPArcCost &cost2)
{ return (cost1.cost == cost2.cost); }

std::ostream &operator<<(std::ostream &os, const SPArcCost &cost)
{ 
  return os<<"Cost="<<cost.cost<<' '<<cost.isValid<<'/'<<cost.backIsValid;
}

