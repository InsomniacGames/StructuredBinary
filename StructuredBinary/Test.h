//
//  Test.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_Test_h
#define StructuredBinary_Test_h

class Test
{
public:
  virtual const char* GetName() const = 0;
  virtual const char* RunTest() const = 0;
};

#endif
