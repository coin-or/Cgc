/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef SPDATA_H
#define SPDATA_H

#include <NodeId.h>

using namespace Cgc;
class SPNodeLabel
{
  int label;
  NodeId pred;
public:
  SPNodeLabel(const int labelVal=0,const NodeId &prev= NodeId(0))
    :label(labelVal),pred(prev){}
  int getLabel(void)const { return label; }
  void setLabel(const int newLabel, const NodeId &previous)
    { label=newLabel; pred=previous; }
  NodeId getPred()const{ return pred; }
  friend bool operator < (const SPNodeLabel &label1, 
			  const SPNodeLabel &label2);
  friend bool operator > (const SPNodeLabel &label1, 
			  const SPNodeLabel &label2);
  friend bool operator != (const SPNodeLabel &label1, 
			   const SPNodeLabel &label2);
  friend bool operator == (const SPNodeLabel &label1, 
			   const SPNodeLabel &label2);
  friend std::ostream &operator<<(std::ostream &os, const SPNodeLabel &label);
};

class SPArcCost 
{
  int cost;
  bool isValid;
  bool backIsValid;
public:
  SPArcCost(const int costVal=999)
    :cost(costVal),
     isValid(true),
     backIsValid(false)
    {}
  int getCost(void)const {return cost;}
  void setCost(const int &newCost){cost=newCost;}

  void makeValid(){ isValid=true;}
  void makeInvalid(){ isValid=false;}
  bool valid()const{ return isValid;}

  void makeBackValid(){ backIsValid=true;}
  void makeBackInvalid(){ backIsValid=false;}
  bool backValid()const{ return backIsValid;}

  friend bool operator < (const SPArcCost &cost1, const SPArcCost &cost2);
  friend bool operator == (const SPArcCost &cost1, const SPArcCost &cost2);
  friend std::ostream &operator<<(std::ostream &os, const SPArcCost &cost);
};

#endif
