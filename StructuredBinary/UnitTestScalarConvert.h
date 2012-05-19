//
//  UnitTestScalarConvert.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestScalarConvert_h
#define StructuredBinary_UnitTestScalarConvert_h

#include "UnitTest.h"

class UnitTestScalarConvert : public UnitTest
{
public:
  virtual const char* GetName() const { return "UnitTestScalarConvert"; }
  virtual const char* RunTest() const;
};

#endif
