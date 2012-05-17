//
//  UnitTestChunkFile.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestChunkFile_h
#define StructuredBinary_UnitTestChunkFile_h

#include "UnitTest.h"

class UnitTestChunkFile : public UnitTest
{
public:
  virtual const char* GetName() const { return "UnitTestChunkFile"; }
  virtual const char* RunTest() const;
};

#endif
