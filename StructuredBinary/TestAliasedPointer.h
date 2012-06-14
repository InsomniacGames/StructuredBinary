//
//  TestAliasedPointer.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestAliasedPointer_h
#define StructuredBinary_TestAliasedPointer_h

#include "UnitTest.h"

class TestAliasedPointer : public UnitTest
{
public:
  virtual const char* GetName() const { return "TestAliasedPointer"; }
  virtual Result      RunTest() const;
};

#endif
