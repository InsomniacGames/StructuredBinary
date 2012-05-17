//
//  UnitTestNumberConvert.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestNumberConvert_h
#define StructuredBinary_UnitTestNumberConvert_h

#include "UnitTest.h"

class UnitTestNumberConvert : public UnitTest
{
public:
  virtual const char* GetName() const { return "UnitTestNumberConvert"; }
  virtual const char* RunTest() const;
};

#endif
