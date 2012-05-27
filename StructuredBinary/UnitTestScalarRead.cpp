//
//  UnitTestScalarRead.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//


// Self
#include "UnitTestScalarRead.h"

// Libraries
#include <cstring>
#include <stdint.h>

// Project
#include "sbScalarValue.h"
#include "sbScalar.h"
#include "sbStruct.h"

//--------------------------------------------------------------------------------------------------

struct ReadStruct
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

const char* UnitTestScalarRead::RunTest() const
{/*
  sbStruct agg( 100, 100 );
  agg.AddScalar( sbFnv32( "f64" ), sbFieldType_ScalarF64 );
  agg.AddScalar( sbFnv32( "i64" ), sbFieldType_ScalarI64 );
  agg.AddScalar( sbFnv32( "u64" ), sbFieldType_ScalarU64 );
  agg.AddScalar( sbFnv32( "f32" ), sbFieldType_ScalarF32 );
  agg.AddScalar( sbFnv32( "i32" ), sbFieldType_ScalarI32 );
  agg.AddScalar( sbFnv32( "u32" ), sbFieldType_ScalarU32 );
  agg.AddScalar( sbFnv32( "i16" ), sbFieldType_ScalarI16 );
  agg.AddScalar( sbFnv32( "u16" ), sbFieldType_ScalarU16 );
  agg.AddScalar( sbFnv32( "i8"  ), sbFieldType_ScalarI8 );
  agg.AddScalar( sbFnv32( "u8"  ), sbFieldType_ScalarU8 );

  ReadStruct s;
  s.f64 = 3.14159265358979;
  s.i64 = 0x69b0aee3db807b41ULL;
  s.u64 = 0x0dbf9bf837f5adb0ULL;
  s.f32 = 2.71828f;
  s.i32 = 0xc4aa8c4a;
  s.u32 = 0x140bbf84;
  s.i16 = 0x4a90;
  s.u16 = 0x6eb6;
  s.i8  = 0xc3;
  s.u8  = 0x40;
  const char* data = ( const char* )&s;
  
  sbScalarValue value;
  n = agg.Read( data, sbFnv32( "f64" ) );
  if( !n.IsFloat() )          return "f64 returned wrong type";
  if( n.AsFloat() != s.f64 )  return "f64 returned wrong value";
  
  n = agg.Read( data, sbFnv32( "i64" ) );
  if( !n.IsInt() )            return "i64 returned wrong type";
  if( n.AsFloat() != s.i64 )  return "i64 returned wrong value";
  
  n = agg.Read( data, sbFnv32( "u64" ) );
  if( !n.IsInt() )            return "u64 returned wrong type";
  if( n.AsFloat() != s.u64 )  return "u64 returned wrong value";
  
  n = agg.Read( data, sbFnv32( "f32" ) );
  if( !n.IsFloat() )          return "f32 returned wrong type";
  if( n.AsFloat() != s.f32 )  return "f32 returned wrong value";
  
  n = agg.Read( data, sbFnv32( "i32" ) );
  if( !n.IsInt() )            return "i32 returned wrong type";
  if( n.AsFloat() != s.i32 )  return "i32 returned wrong value";
  
  n = agg.Read( data, sbFnv32( "u32" ) );
  if( !n.IsInt() )            return "u32 returned wrong type";
  if( n.AsFloat() != s.u32 )  return "u32 returned wrong value";
  
  n = agg.Read( data, sbFnv32( "i16" ) );
  if( !n.IsInt() )            return "i16 returned wrong type";
  if( n.AsFloat() != s.i16 )  return "i16 returned wrong value";
  
  n = agg.Read( data, sbFnv32( "u8" ) );
  if( !n.IsInt() )            return "u8 returned wrong type";
  if( n.AsFloat() != s.u8 )   return "u8 returned wrong value";
*/  
  return NULL;
}

//--------------------------------------------------------------------------------------------------
