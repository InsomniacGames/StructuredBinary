//
//  ScalarReadUnitTest.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//


// Self
#include "ScalarReadUnitTest.h"

// Libraries
#include <cstring>
#include <stdint.h>

// Project
#include "Number.h"
#include "Field.h"
#include "Aggregate.h"

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

const char* ScalarReadUnitTest::RunTest() const
{
  Aggregate agg( 10 );
  agg.SetFloat64( 0,  0, "f64" );
  agg.SetInt64  ( 1,  8, "i64" );
  agg.SetUInt64 ( 2, 16, "u64" );
  agg.SetFloat32( 3, 24, "f32" );
  agg.SetInt32  ( 4, 28, "i32" );
  agg.SetUInt32 ( 5, 32, "u32" );
  agg.SetInt16  ( 6, 36, "i16" );
  agg.SetUInt16 ( 7, 38, "u16" );
  agg.SetInt8   ( 8, 40, "i8"  );
  agg.SetUInt8  ( 9, 41, "u8"  );
  
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
  
  Number n;
  n = agg.Read( data, "f64" );
  if( !n.IsFloat() )          return "f64 returned wrong type";
  if( n.AsFloat() != s.f64 )  return "f64 returned wrong value";
  
  n = agg.Read( data, "i64" );
  if( !n.IsInt() )            return "i64 returned wrong type";
  if( n.AsFloat() != s.i64 )  return "i64 returned wrong value";
  
  n = agg.Read( data, "u64" );
  if( !n.IsInt() )            return "u64 returned wrong type";
  if( n.AsFloat() != s.u64 )  return "u64 returned wrong value";
  
  n = agg.Read( data, "f32" );
  if( !n.IsFloat() )          return "f32 returned wrong type";
  if( n.AsFloat() != s.f32 )  return "f32 returned wrong value";
  
  n = agg.Read( data, "i32" );
  if( !n.IsInt() )            return "i32 returned wrong type";
  if( n.AsFloat() != s.i32 )  return "i32 returned wrong value";
  
  n = agg.Read( data, "u32" );
  if( !n.IsInt() )            return "u32 returned wrong type";
  if( n.AsFloat() != s.u32 )  return "u32 returned wrong value";
  
  n = agg.Read( data, "i16" );
  if( !n.IsInt() )            return "i16 returned wrong type";
  if( n.AsFloat() != s.i16 )  return "i16 returned wrong value";
  
  n = agg.Read( data, "u8" );
  if( !n.IsInt() )            return "u8 returned wrong type";
  if( n.AsFloat() != s.u8 )   return "u8 returned wrong value";
  
  return NULL;
}

//--------------------------------------------------------------------------------------------------
