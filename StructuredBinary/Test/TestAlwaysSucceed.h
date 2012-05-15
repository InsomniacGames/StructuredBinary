//
//  TestAlwaysSucceed.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestAlwaysSucceed_h
#define StructuredBinary_TestAlwaysSucceed_h

#include "Test.h"

/*
 This is a control test
 */
class TestAlwaysSucceed : public Test
{
public:
  virtual const char* GetName() const { return "AlwaysSucceed"; }
  virtual const char* RunTest() const;
};

#endif
