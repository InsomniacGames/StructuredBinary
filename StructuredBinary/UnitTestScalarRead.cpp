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

const char* UnitTestScalarRead::RunTest() const
{
  Aggregate agg( 10 );
  agg.AddField( Fnv32( "f64" ), kField_F64 );
  agg.AddField( Fnv32( "i64" ), kField_I64 );
  agg.AddField( Fnv32( "u64" ), kField_U64 );
  agg.AddField( Fnv32( "f32" ), kField_F32 );
  agg.AddField( Fnv32( "i32" ), kField_I32 );
  agg.AddField( Fnv32( "u32" ), kField_U32 );
  agg.AddField( Fnv32( "i16" ), kField_I16 );
  agg.AddField( Fnv32( "u16" ), kField_U16 );
  agg.AddField( Fnv32( "i8"  ), kField_I8 );
  agg.AddField( Fnv32( "u8"  ), kField_U8 );
  agg.FixSizeAndStride();

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
  n = agg.Read( data, Fnv32( "f64" ) );
  if( !n.IsFloat() )          return "f64 returned wrong type";
  if( n.AsFloat() != s.f64 )  return "f64 returned wrong value";
  
  n = agg.Read( data, Fnv32( "i64" ) );
  if( !n.IsInt() )            return "i64 returned wrong type";
  if( n.AsFloat() != s.i64 )  return "i64 returned wrong value";
  
  n = agg.Read( data, Fnv32( "u64" ) );
  if( !n.IsInt() )            return "u64 returned wrong type";
  if( n.AsFloat() != s.u64 )  return "u64 returned wrong value";
  
  n = agg.Read( data, Fnv32( "f32" ) );
  if( !n.IsFloat() )          return "f32 returned wrong type";
  if( n.AsFloat() != s.f32 )  return "f32 returned wrong value";
  
  n = agg.Read( data, Fnv32( "i32" ) );
  if( !n.IsInt() )            return "i32 returned wrong type";
  if( n.AsFloat() != s.i32 )  return "i32 returned wrong value";
  
  n = agg.Read( data, Fnv32( "u32" ) );
  if( !n.IsInt() )            return "u32 returned wrong type";
  if( n.AsFloat() != s.u32 )  return "u32 returned wrong value";
  
  n = agg.Read( data, Fnv32( "i16" ) );
  if( !n.IsInt() )            return "i16 returned wrong type";
  if( n.AsFloat() != s.i16 )  return "i16 returned wrong value";
  
  n = agg.Read( data, Fnv32( "u8" ) );
  if( !n.IsInt() )            return "u8 returned wrong type";
  if( n.AsFloat() != s.u8 )   return "u8 returned wrong value";
  
  return NULL;
}

//--------------------------------------------------------------------------------------------------
