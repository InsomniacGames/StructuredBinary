//
//  TestScalarRead.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "TestScalarRead.h"

// Libraries
#include <cstring>
#include <stdint.h>

// Project

//--------------------------------------------------------------------------------------------------

struct ScalarReadStruct
{
  double    f64;
  int64_t   i64;
  uint64_t  u64;
  float     f32;
  int32_t   i32;
  uint32_t  u32;
  int16_t   i16;
  uint16_t  u16;
  int8_t    i8;
  uint8_t   u8;
};

//--------------------------------------------------------------------------------------------------

class Field
{
public:
};



const char* TestScalarRead::RunTest() const
{
  return NULL;
}

//--------------------------------------------------------------------------------------------------
