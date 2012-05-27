//
//  UnitTestPointer.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/27/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestPointer_h
#define StructuredBinary_UnitTestPointer_h

#include "UnitTest.h"

class UnitTestPointer : public UnitTest
{
public:
  virtual const char* GetName() const { return "UnitTestPointer"; }
  virtual const char* RunTest() const;
};


#endif
