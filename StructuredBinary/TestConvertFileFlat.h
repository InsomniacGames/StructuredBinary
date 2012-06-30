//
//  TestConvertFileFlat.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/30/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestConvertFileFlat_h
#define StructuredBinary_TestConvertFileFlat_h

#include "UnitTest.h"

class TestConvertFileFlat : public UnitTest
{
public:
  virtual const char* GetName() const { return "TestConvertFileFlat"; }
  virtual Result      RunTest() const;

private:
  Result TestRead() const;
  Result TestWrite() const;
};

#endif
