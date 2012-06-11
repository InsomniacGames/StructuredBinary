//
//  TestConvertDeep.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/10/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestConvertDeep_h
#define StructuredBinary_TestConvertDeep_h

#include "UnitTest.h"

class TestConvertDeep : public UnitTest
{
public:
  virtual const char* GetName() const { return "TestConvertDeep"; }
  virtual Result      RunTest() const;
};

#endif
