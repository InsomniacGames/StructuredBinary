//
//  TestScalarConvert.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestScalarConvert_h
#define StructuredBinary_TestScalarConvert_h

#include "Test.h"

class TestScalarConvert : public Test
{
public:
  virtual const char* GetName() const { return "Scalar Convert"; }
  virtual const char* RunTest() const;
};


#endif
