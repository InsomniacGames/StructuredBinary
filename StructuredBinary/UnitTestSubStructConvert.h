//
//  UnitTestSubStructConvert.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/15/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestSubStructConvert_h
#define StructuredBinary_UnitTestSubStructConvert_h

#include "UnitTest.h"

class UnitTestSubStructConvert : public UnitTest
{
public:
  virtual const char* GetName() const { return "UnitTestSubStructConvert"; }
  virtual const char* RunTest() const;
};

#endif
