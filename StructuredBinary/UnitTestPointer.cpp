//
//  UnitTestPointer.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/27/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "UnitTestPointer.h"

// Libraries
#include <cstring>
#include <stdint.h>
#include <stdio.h>

// Project
#include "sbScalarValue.h"
#include "sbScalar.h"
#include "sbStruct.h"

//--------------------------------------------------------------------------------------------------

struct OtherStruct
{
  uint32_t  var1;
  uint32_t  var2;
};

struct ReadStruct
{
  uint32_t            var3;
  const OtherStruct*  ptr4;
};

struct awkward
{
  uint64_t u64;
  uint8_t  u8;
};

struct outer
{
  awkward aw;
  uint8_t u8;
};

//--------------------------------------------------------------------------------------------------

const char* UnitTestPointer::RunTest() const
{
  sbStruct src_agg;
  src_agg.AddScalar( sbFnv32( "array1" ), sbFieldType_U8, 3 );
  src_agg.AddScalar( sbFnv32( "array2" ), sbFieldType_U8, 3 );
  
  sbStruct dst_agg;
  dst_agg.AddScalar( sbFnv32( "array2" ), sbFieldType_U8, 3 );
  dst_agg.AddScalar( sbFnv32( "array1" ), sbFieldType_U8, 3 );

  OtherStruct other_struct;
  other_struct.var1 = 0xD1423A2C;
  other_struct.var2 = 0x14CB8227;
  
  ReadStruct read_struct;
  read_struct.var3 = 0xE026AE6B;
  read_struct.ptr4 = &other_struct;

  outer o;
  const char* read_data = ( const char* )&read_struct;
  int offset = ( int )( ( char* )&o.u8 - ( char* )&o );

//  if(  )

  return NULL;
}
