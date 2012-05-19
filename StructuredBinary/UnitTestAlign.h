//
//  UnitTestAlign.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/19/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestAlign_h
#define StructuredBinary_UnitTestAlign_h

#include "UnitTest.h"

class UnitTestAlign : public UnitTest
{
public:
  virtual const char* GetName() const { return "UnitTestAlign"; }
  virtual const char* RunTest() const;
};

#endif
