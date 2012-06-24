//
//  TestSchemaSerialize.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/23/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_TestSchemaSerialize_h
#define StructuredBinary_TestSchemaSerialize_h

#include "UnitTest.h"

class TestSchemaSerialize : public UnitTest
{
public:
  virtual const char* GetName() const { return "TestSchemaSerialize"; }
  virtual Result      RunTest() const;
};

#endif
