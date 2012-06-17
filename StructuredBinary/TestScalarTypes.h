//
//  TestScalarTypes.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestScalarTypes_h
#define StructuredBinary_TestScalarTypes_h

#include "UnitTest.h"

class TestScalarTypes : public UnitTest
{
public:
  virtual const char* GetName() const { return "TestScalarTypes"; }
  virtual Result      RunTest() const;
};

#endif
