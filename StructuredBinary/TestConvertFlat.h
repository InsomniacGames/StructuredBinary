//
//  TestConvertFlat.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/9/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestConvertFlat_h
#define StructuredBinary_TestConvertFlat_h

#include "UnitTest.h"

class TestConvertFlat : public UnitTest
{
public:
  virtual const char* GetName() const { return "TestConvertFlat"; }
  virtual Result      RunTest() const;
};


#endif
