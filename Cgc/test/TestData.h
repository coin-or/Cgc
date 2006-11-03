/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef TESTDATA_H
#define TESTDATA_H

#include <iostream>

class NodeLabel
{
  int val;
public:
  NodeLabel():val(99){}
  NodeLabel(const int intVal):val(intVal){}
  int getVal()const{ return val;}
  void setVal(const int &newVal){val=newVal ;}
  friend std::ostream &operator<<(std::ostream &os, const NodeLabel &nodeI);
  inline friend bool operator!=(const NodeLabel &lab1, const NodeLabel &lab2)
    { return lab1.val != lab2.val; }
  inline friend bool operator==(const NodeLabel &lab1, const NodeLabel &lab2)
    { return lab1.val == lab2.val; }
  inline friend bool operator<(const NodeLabel &lab1, const NodeLabel &lab2)
    { return lab1.val < lab2.val; }
};

class ArcCost
{
  int val;
public:
  ArcCost():val(98){}
  ArcCost(const int intVal):val(intVal){}
  ArcCost(const ArcCost &otherCost):val(otherCost.val){}
  int getVal()const{ return val;}
  void setVal(const int &newVal){val=newVal ;}
  inline friend bool operator<(const ArcCost &arc1, const ArcCost &arc2)
    { return arc1.val < arc2.val; }
  inline friend bool operator==(const ArcCost &cst1, const ArcCost &cst2)
    { return cst1.val == cst2.val; }
  inline friend bool operator!=(const ArcCost &cst1, const ArcCost &cst2)
    { return cst1.val != cst2.val; }
  friend std::ostream &operator<<(std::ostream &os, const ArcCost &cst);
};

#endif
