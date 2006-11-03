/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef MSTDATA_H
#define MSTDATA_H

#include <NodeId.h>
using namespace Cgc;
class MSTNodeLabel
{
  NodeId parent;
public:
  MSTNodeLabel(const NodeId &prev= NodeId(0))
    :parent(prev){}
  NodeId getParent()const{ return parent; }
  void setParent(const NodeId &newParent){ parent= newParent; }
  friend bool operator < (const MSTNodeLabel &label1, const MSTNodeLabel &label2);

  friend bool operator==(const MSTNodeLabel &label1,const MSTNodeLabel &label2);
  friend bool operator!=(const MSTNodeLabel &label1,const MSTNodeLabel &label2);

  const MSTNodeLabel &operator=(const MSTNodeLabel &otherlabel)
    {parent = otherlabel.parent; return *this;}
  
  friend std::ostream &operator<<(std::ostream &os, const MSTNodeLabel &label);
};

class MSTArcCost 
{
  int cost;
public:
  MSTArcCost(const int costVal=999):cost(costVal){}
  int getCost(void)const {return cost;}
  friend bool operator < (const MSTArcCost &cost1, const MSTArcCost &cost2);
  friend bool operator == (const MSTArcCost &cost1, const MSTArcCost &cost2);
  friend std::ostream &operator<<(std::ostream &os, const MSTArcCost &cost);
};

#endif
