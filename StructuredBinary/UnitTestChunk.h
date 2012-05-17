//
//  UnitTestChunk.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestChunk_h
#define StructuredBinary_UnitTestChunk_h

#include "UnitTest.h"

class UnitTestChunk : public UnitTest
{
public:
  virtual const char* GetName() const { return "UnitTestChunk"; }
  virtual const char* RunTest() const;
};


#endif
