/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#include "unitTest.h"

int main()
{
    {
        TestBed myBed;

        BFTraversalTest(myBed);
        ConnCompTest(myBed);
        DFTraversalTest(myBed);
        StaticNetTest(myBed);
        DynNetTest(myBed);
        LayerSPTest(myBed);
        MSTDataTest(myBed);
        MSTTest(myBed);
        NodeIdTest(myBed);
        PathFindTest(myBed);
        PathTest(myBed);
        SPDynTest(myBed);
        SPStaticTest(myBed);
        SPTest(myBed);
        SPTestDataTest(myBed);
        SPStaticFBTest(myBed);
        SSPSolverTest(myBed);
        StaticFBNetTest(myBed);
    }
    return 0;
}
