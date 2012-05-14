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
#include "Number.h"
#include "Field.h"
#include "Aggregate.h"

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

FieldFloat32  g_Float32;
FieldFloat64  g_Float64;
FieldInt64    g_Int64;
FieldInt32    g_Int32;
FieldInt16    g_Int16;
FieldInt8     g_Int8;
FieldUInt64   g_UInt64;
FieldUInt32   g_UInt32;
FieldUInt16   g_UInt16;
FieldUInt8    g_UInt8;


//--------------------------------------------------------------------------------------------------

const char* TestScalarRead::RunTest() const
{
  Aggregate agg( 10 );
  agg.SetField( 0,  0, "f64", &g_Float64 );
  agg.SetField( 1,  8, "i64", &g_Int64 );
  agg.SetField( 2, 16, "u64", &g_UInt64 );
  agg.SetField( 3, 24, "f32", &g_Float32 );
  agg.SetField( 4, 28, "i32", &g_Int32 );
  agg.SetField( 5, 32, "u32", &g_UInt32 );
  agg.SetField( 6, 36, "i16", &g_Int16 );
  agg.SetField( 7, 38, "u16", &g_UInt16 );
  agg.SetField( 8, 40, "i8",  &g_Int8 );
  agg.SetField( 9, 41, "u8",  &g_UInt8 );
  
  ScalarReadStruct s;
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
