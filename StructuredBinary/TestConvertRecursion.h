//
//  TestConvertRecursion.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestConvertRecursion_h
#define StructuredBinary_TestConvertRecursion_h

#include "UnitTest.h"

class TestConvertRecursion : public UnitTest
{
public:
  virtual const char* GetName() const { return "TestConvertRecursion"; }
  virtual Result      RunTest() const;
};

#endif
