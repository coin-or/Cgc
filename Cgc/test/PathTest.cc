/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <iostream>
#include "TestItem.h"
#include "TestBed.h"
#include <NodeId.h>
#include <ArcPath.h>
#include <Path.h>

static void test1()
{
	TestItem *item1 = new TestItem("PathTest:1:Construct");
	NodeId nodeId1(1);
	NodeId nodeId2(2);
	Path path;
	path.push_back(nodeId1);
	path.push_back(nodeId2);
	path.setCost(2);
	//std::cout<<"Path="<<path<<std::endl;
	item1->passItem();
}

static void test2()
{
  TestItem *ti = new TestItem("PathTest:2:Construct ArcPath");
  ArcPath<int> ap;
  ap.setCost(3);
  //std::cout<<"ArcPath:"<<ap<<std::endl;
  ti->passItem();
}
static void test3()
{
    TestItem *ti = new TestItem("PathTest:3:reverse");
	NodeId nodeId1(1);
	NodeId nodeId2(2);
	Path path;
	path.push_back(nodeId1);
	path.push_back(nodeId2);
	path.setCost(2);
    //std::cout<<std::endl;
	//std::cout<<"Path="<<path<<std::endl;
    path.reverse();
	//std::cout<<"Path Reversed ="<<path<<std::endl;
    //std::cout<<std::endl;
	ti->passItem();
}

int PathTest(TestBed &myBed)
{
	TestItem::setBed(&myBed);

	test1(); 
	test2(); 
	test3(); 
	return 0;
}
