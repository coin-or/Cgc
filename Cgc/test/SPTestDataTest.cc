/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <iostream>
#include "SPTest.h"
#include "TestItem.h"
#include "TestBed.h"

static void test1()
{
	TestItem *item1 = new TestItem("SPTestDataTest:1:Construct");
	SPNodeLabel a_label;
	SPArcCost anArc;
	//std::cout<<"Node:"<<a_label<<std::endl;
	//std::cout<<"Arc:"<<anArc<<std::endl;
	item1->passItem();
}

static void test2()
{
  TestItem *ti = new TestItem("SPTestDataTest:2: Test validation routines");
  SPArcCost anArc;
  // test forward direction
  if(anArc.valid())
    ti->passItem();
  else
    ti->failItem(__SPOT__);

  anArc.makeInvalid();
  if(anArc.valid())
    ti->failItem(__SPOT__);

  anArc.makeValid();
  if(!anArc.valid())
    ti->failItem(__SPOT__);

  // test backward direction
  if(anArc.backValid())
    ti->failItem(__SPOT__);

  anArc.makeBackInvalid();
  if(anArc.backValid())
    ti->failItem(__SPOT__);

  anArc.makeBackValid();
  if(!anArc.backValid())
    ti->failItem(__SPOT__);

}

static void test3()
{
  TestItem *ti = new TestItem("SPTestDataTest:3:arc getter/setter");
  int value=34;
  SPArcCost anArc;
  anArc.setCost(value);
  if(anArc.getCost()!=value)
    ti->failItem(__SPOT__);
  else
    ti->passItem();
  anArc.setCost(value+value);
  int value2=anArc.getCost();
  if(value2!=value+value)
    ti->failItem(__SPOT__);
}


int SPTestDataTest(TestBed &myBed)
{
  
  TestItem::setBed(&myBed);
  
  test1(); 
  test2();
  test3();
  return 0;
}
