//
//  UnitTestScalarRead.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestScalarRead_h
#define StructuredBinary_UnitTestScalarRead_h

#include "UnitTest.h"

class UnitTestScalarRead : public UnitTest
{
public:
  virtual const char* GetName() const { return "UnitTestScalarRead"; }
  virtual const char* RunTest() const;
};


#endif
