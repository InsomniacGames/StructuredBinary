//
//  UnitTestScalarConvert.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "UnitTestScalarConvert.h"

// Libraries
#include <cstring>
#include <stdint.h>
#include <stdio.h>

// Project
#include "sbScalarValue.h"
#include "sbScalar.h"
#include "sbStruct.h"

//--------------------------------------------------------------------------------------------------

struct SourceStruct
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

struct DestStruct
{
  uint64_t  u64;
  int64_t   i64;
  double    f64;
  uint32_t  u32;
  int32_t   i32;
  float     f32;
  uint8_t   u8;
  int8_t    i8;
  uint16_t  u16;
  int16_t   i16;
};

//--------------------------------------------------------------------------------------------------

const char* UnitTestScalarConvert::RunTest() const
{
  sbStruct src_agg;
  src_agg.AddScalar( sbFnv32( "f64" ), sbFieldType_F64 );
  src_agg.AddScalar( sbFnv32( "i64" ), sbFieldType_I64 );
  src_agg.AddScalar( sbFnv32( "u64" ), sbFieldType_U64 );
  src_agg.AddScalar( sbFnv32( "f32" ), sbFieldType_F32 );
  src_agg.AddScalar( sbFnv32( "i32" ), sbFieldType_I32 );
  src_agg.AddScalar( sbFnv32( "u32" ), sbFieldType_U32 );
  src_agg.AddScalar( sbFnv32( "i16" ), sbFieldType_I16 );
  src_agg.AddScalar( sbFnv32( "u16" ), sbFieldType_U16 );
  src_agg.AddScalar( sbFnv32( "i8"  ), sbFieldType_I8  );
  src_agg.AddScalar( sbFnv32( "u8"  ), sbFieldType_U8  );

  sbStruct dest_agg;
  dest_agg.AddScalar( sbFnv32( "u64" ), sbFieldType_U64 );
  dest_agg.AddScalar( sbFnv32( "i64" ), sbFieldType_I64 );
  dest_agg.AddScalar( sbFnv32( "f64" ), sbFieldType_F64 );
  dest_agg.AddScalar( sbFnv32( "u32" ), sbFieldType_U32 );
  dest_agg.AddScalar( sbFnv32( "i32" ), sbFieldType_I32 );
  dest_agg.AddScalar( sbFnv32( "f32" ), sbFieldType_F32 );
  dest_agg.AddScalar( sbFnv32( "u8"  ), sbFieldType_U8  );
  dest_agg.AddScalar( sbFnv32( "i8"  ), sbFieldType_I8  );
  dest_agg.AddScalar( sbFnv32( "u16" ), sbFieldType_U16 );
  dest_agg.AddScalar( sbFnv32( "i16" ), sbFieldType_I16 );

  SourceStruct src;
  src.f64 = 3.14159265358979;
  src.i64 = 0x69b0aee3db807b41ULL;
  src.u64 = 0x0dbf9bf837f5adb0ULL;
  src.f32 = 2.71828f;
  src.i32 = 0xc4aa8c4a;
  src.u32 = 0x140bbf84;
  src.i16 = 0x4a90;
  src.u16 = 0x6eb6;
  src.i8  = 0xc3;
  src.u8  = 0x40;

  DestStruct dest;

  const char* src_data = ( const char* )&src;
  char* dest_data = ( char* )&dest;
  
  dest_agg.Convert( dest_data, src_data, &src_agg );

  if( dest.f64 != src.f64 )  return "f64 converted incorrectly";
  if( dest.f32 != src.f32 )  return "f32 converted incorrectly";
  if( dest.i64 != src.i64 )  return "i64 converted incorrectly";
  if( dest.u64 != src.u64 )  return "u64 converted incorrectly";
  if( dest.i32 != src.i32 )  return "i32 converted incorrectly";
  if( dest.u32 != src.u32 )  return "u32 converted incorrectly";
  if( dest.i16 != src.i16 )  return "i16 converted incorrectly";
  if( dest.u16 != src.u16 )  return "u16 converted incorrectly";
  if( dest.i8  != src.i8  )  return "i8 converted incorrectly";
  if( dest.u8  != src.u8  )  return "u8 converted incorrectly";
    
  return NULL;
}
