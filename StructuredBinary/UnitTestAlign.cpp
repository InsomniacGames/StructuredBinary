//
//  UnitTestAlign.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/19/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include <iostream>

// Self
#include "UnitTestAlign.h"

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
  uint8_t   u8;
  double    f64;
  uint16_t  u16;
  int32_t   i32;
  int64_t   i64;
  uint64_t  u64;
  int8_t    i8;
  float     f32;
  uint32_t  u32;
  int16_t   i16;
};

struct DestStruct
{                 // Offset
  uint8_t   u8;   //  0
  uint64_t  u64;  //  8
  int64_t   i64;  // 16
  uint16_t  u16;  // 24
  double    f64;  // 32
  int8_t    i8;   // 40
  uint32_t  u32;  // 44
  int32_t   i32;  // 48
  float     f32;  // 52
  int16_t   i16;  // 56
};                // 58

//--------------------------------------------------------------------------------------------------

const char* UnitTestAlign::RunTest() const
{
  sbStruct src_agg( 100, 100 );
  src_agg.AddScalar( sbFnv32( "u8"  ), sbFieldType_U8  );
  src_agg.AddScalar( sbFnv32( "f64" ), sbFieldType_F64 );
  src_agg.AddScalar( sbFnv32( "u16" ), sbFieldType_U16 );
  src_agg.AddScalar( sbFnv32( "i32" ), sbFieldType_I32 );
  src_agg.AddScalar( sbFnv32( "i64" ), sbFieldType_I64 );
  src_agg.AddScalar( sbFnv32( "u64" ), sbFieldType_U64 );
  src_agg.AddScalar( sbFnv32( "i8"  ), sbFieldType_I8  );
  src_agg.AddScalar( sbFnv32( "f32" ), sbFieldType_F32 );
  src_agg.AddScalar( sbFnv32( "u32" ), sbFieldType_U32 );
  src_agg.AddScalar( sbFnv32( "i16" ), sbFieldType_I16 );
  
  sbStruct dst_agg( 100, 100 );
  dst_agg.AddScalar( sbFnv32( "u8"  ), sbFieldType_U8  );
  dst_agg.AddScalar( sbFnv32( "u64" ), sbFieldType_U64 );
  dst_agg.AddScalar( sbFnv32( "i64" ), sbFieldType_I64 );
  dst_agg.AddScalar( sbFnv32( "u16" ), sbFieldType_U16 );
  dst_agg.AddScalar( sbFnv32( "f64" ), sbFieldType_F64 );
  dst_agg.AddScalar( sbFnv32( "i8"  ), sbFieldType_I8  );
  dst_agg.AddScalar( sbFnv32( "u32" ), sbFieldType_U32 );
  dst_agg.AddScalar( sbFnv32( "i32" ), sbFieldType_I32 );
  dst_agg.AddScalar( sbFnv32( "f32" ), sbFieldType_F32 );
  dst_agg.AddScalar( sbFnv32( "i16" ), sbFieldType_I16 );

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
  
  DestStruct dst;
  
  const char* src_data = ( const char* )&src;
  char* dst_data = ( char* )&dst;

  dst_agg.Convert( dst_data, src_data, &src_agg );
  
  if( dst.u8  != src.u8  )  return "u8 converted incorrectly";
  if( dst.i8  != src.i8  )  return "i8 converted incorrectly";
  if( dst.f64 != src.f64 )  return "f64 converted incorrectly";
  if( dst.f32 != src.f32 )  return "f32 converted incorrectly";
  if( dst.i64 != src.i64 )  return "i64 converted incorrectly";
  if( dst.u64 != src.u64 )  return "u64 converted incorrectly";
  if( dst.i32 != src.i32 )  return "i32 converted incorrectly";
  if( dst.u32 != src.u32 )  return "u32 converted incorrectly";
  if( dst.i16 != src.i16 )  return "i16 converted incorrectly";
  if( dst.u16 != src.u16 )  return "u16 converted incorrectly";
  
  return NULL;
}
