//
//  ScalarReadUnitTest.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_ScalarReadUnitTest_h
#define StructuredBinary_ScalarReadUnitTest_h

#include "Test.h"

class ScalarReadUnitTest : public Test
{
public:
  virtual const char* GetName() const { return "ScalarReadUnitTest"; }
  virtual const char* RunTest() const;
};


#endif
