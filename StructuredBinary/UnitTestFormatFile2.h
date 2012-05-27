//
//  UnitTestFormatFile2.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/27/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestFormatFile2_h
#define StructuredBinary_UnitTestFormatFile2_h

#include "UnitTest.h"

class UnitTestFormatFile2 : public UnitTest
{
public:
  virtual const char* GetName() const { return "UnitTestFormatFile2"; }
  virtual const char* RunTest() const;
};

#endif
