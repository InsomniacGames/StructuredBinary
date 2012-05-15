//
//  TestNumberConvert.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestNumberConvert_h
#define StructuredBinary_TestNumberConvert_h

#include "Test.h"

class TestNumberConvert : public Test
{
public:
  virtual const char* GetName() const { return "Number Convert"; }
  virtual const char* RunTest() const;
};

#endif
