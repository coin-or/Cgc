/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef UNITTEST_H
#define UNITTEST_H
#include "TestBed.h"
#include "TestItem.h"
int BFTraversalTest(TestBed &bed);
int ConnCompTest(TestBed &bed);
int DFTraversalTest(TestBed &bed);
int DynNetTest(TestBed &bed);
int LayerSPTest(TestBed &bed);
int MSTDataTest(TestBed &bed);
int MSTTest(TestBed &bed);
int NodeIdTest(TestBed &bed);
int PathFindTest(TestBed &bed);
int PathTest(TestBed &bed);
//int SAPEKTest(TestBed &bed);
int SPDynTest(TestBed &bed);
int SPStaticFBTest(TestBed &bed);
int SPStaticTest(TestBed &bed);
int SPTest(TestBed &bed);
int SPTestDataTest(TestBed &bed);
int SSPSolverTest(TestBed &bed);
int StaticFBNetTest(TestBed &bed);
int StaticNetTest(TestBed &bed);
void DotOutputFormatTest(TestBed &bed);
void ReadDimacsTest(TestBed &bed);
void SPLabelCorrTest(TestBed &bed);

#endif
