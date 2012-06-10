//
//  UnitTestFormatFile.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/19/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestFormatFile_h
#define StructuredBinary_UnitTestFormatFile_h

#include "UnitTest.h"

class TestFormatFile : public UnitTest
{
public:
  virtual const char* GetName() const { return "UnitTestFormatFile"; }
  virtual Result      RunTest() const;
};

#endif
