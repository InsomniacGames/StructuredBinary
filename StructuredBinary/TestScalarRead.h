//
//  TestScalarRead.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestScalarRead_h
#define StructuredBinary_TestScalarRead_h

#include "Test.h"

class TestScalarRead : public Test
{
public:
  virtual const char* GetName() const { return "ScalarRead"; }
  virtual const char* RunTest() const;
};

#endif
