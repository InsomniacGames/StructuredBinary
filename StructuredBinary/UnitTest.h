//
//  UnitTest.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTest_h
#define StructuredBinary_UnitTest_h

class UnitTest
{
public:
  virtual const char* GetName() const = 0;
  virtual const char* RunTest() const = 0;
  const char* Run() const;
};

#endif
