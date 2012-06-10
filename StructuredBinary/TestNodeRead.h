//
//  TestNodeRead.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/3/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestNodeRead_h
#define StructuredBinary_TestNodeRead_h

#include "UnitTest.h"

class TestNodeRead : public UnitTest
{
public:
  virtual const char* GetName() const { return "TestNodeRead"; }
  virtual Result      RunTest() const;
};

#endif
