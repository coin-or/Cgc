/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef TESTBED_H
#define TESTBED_H
#include <iostream>
#include <set>
#include <PLess.h>
#include "TestItem.h"
using namespace Cgc;
class TestBed : public std::set < TestItem * , PLess< TestItem * > >
{
public:
	TestBed(){ std::cout<<"Test begin"<<std::endl;}
	~TestBed(){ std::cout<<"Test end\nresults:\n"<<*this<<std::endl;}
  	friend std::ostream &operator<<(std::ostream &os, const TestBed &bed);
};
#endif
