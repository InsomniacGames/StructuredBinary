//
//  TestScalarConvert.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "TestScalarConvert.h"

// Libraries
#include <cstring>
#include <stdint.h>

// Project
#include "Number.h"
#include "Field.h"
#include "Aggregate.h"

//--------------------------------------------------------------------------------------------------

struct ScalarConvertSourceStruct
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

struct ScalarConvertDestStruct
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

extern FieldFloat32  g_Float32;
extern FieldFloat64  g_Float64;
extern FieldInt64    g_Int64;
extern FieldInt32    g_Int32;
extern FieldInt16    g_Int16;
extern FieldInt8     g_Int8;
extern FieldUInt64   g_UInt64;
extern FieldUInt32   g_UInt32;
extern FieldUInt16   g_UInt16;
extern FieldUInt8    g_UInt8;

//--------------------------------------------------------------------------------------------------

const char* TestScalarConvert::RunTest() const
{
  Aggregate src_agg( 10 );
  src_agg.SetField( 0,  0, "f64", &g_Float64 );
  src_agg.SetField( 1,  8, "i64", &g_Int64 );
  src_agg.SetField( 2, 16, "u64", &g_UInt64 );
  src_agg.SetField( 3, 24, "f32", &g_Float32 );
  src_agg.SetField( 4, 28, "i32", &g_Int32 );
  src_agg.SetField( 5, 32, "u32", &g_UInt32 );
  src_agg.SetField( 6, 36, "i16", &g_Int16 );
  src_agg.SetField( 7, 38, "u16", &g_UInt16 );
  src_agg.SetField( 8, 40, "i8",  &g_Int8 );
  src_agg.SetField( 9, 41, "u8",  &g_UInt8 );
  
  Aggregate dest_agg( 10 );
  dest_agg.SetField( 0,  0, "u64", &g_UInt64 );
  dest_agg.SetField( 1,  8, "i64", &g_Int64 );
  dest_agg.SetField( 2, 16, "f64", &g_Float64 );
  dest_agg.SetField( 3, 24, "u32", &g_UInt32 );
  dest_agg.SetField( 4, 28, "i32", &g_Int32 );
  dest_agg.SetField( 5, 32, "f32", &g_Float32 );
  dest_agg.SetField( 6, 36, "u8",  &g_UInt8 );
  dest_agg.SetField( 7, 37, "i8",  &g_Int8 );
  dest_agg.SetField( 8, 38, "u16", &g_UInt16 );
  dest_agg.SetField( 9, 40, "i16", &g_Int16 );
  
  ScalarConvertSourceStruct src;
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

  ScalarConvertDestStruct dest;

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
