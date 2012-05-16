//
//  TestSubStructConvert.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/15/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestSubStructConvert_h
#define StructuredBinary_TestSubStructConvert_h

#include "Test.h"

class TestSubStructConvert : public Test
{
public:
  virtual const char* GetName() const { return "SubStruct Convert"; }
  virtual const char* RunTest() const;
};

#endif
