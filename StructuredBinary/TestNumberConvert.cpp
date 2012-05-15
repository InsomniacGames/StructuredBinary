//
//  TestNumberConvert.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "TestNumberConvert.h"

// Libraries
#include <cstring>
#include <stdint.h>

// Project
#include "Number.h"

//--------------------------------------------------------------------------------------------------

const char* TestNumberConvert::RunTest() const
{
  Number n;
  
  int8_t    i8 = -1;
  uint8_t   u8 = 0xff;
  int16_t   i16 = -1;
  uint16_t  u16 = 0xffff;
  int32_t   i32 = -1;
  uint32_t  u32 = 0xffffffff;
  int64_t   i64 = -1LL;
  uint64_t  u64 = 0xffffffffffffffffULL;
  float     f32 = -1.0f;
  double    f64 = -1.0;

  n = Number::Int( i8 );
  if( n.AsInt() != -1LL )                   return "i8 did not convert correctly";
  if( n.AsFloat() != -1.0 )                 return "i8 -> float did not convert correctly";
  n = Number::Int( u8 );
  if( n.AsInt() != 0x00000000000000ffLL )   return "u8 did not convert correctly";
  
  n = Number::Int( i16 );
  if( n.AsInt() != -1LL )                   return "i16 did not convert correctly";
  if( n.AsFloat() != -1.0 )                 return "i16 -> float did not convert correctly";
  n = Number::Int( u16 );
  if( n.AsInt() != 0x000000000000ffffLL )   return "u16 did not convert correctly";
  
  n = Number::Int( i32 );
  if( n.AsInt() != -1LL )                   return "i32 did not convert correctly";
  if( n.AsFloat() != -1.0 )                 return "i32 -> float did not convert correctly";
  n = Number::Int( u32 );
  if( n.AsInt() != 0x00000000ffffffffLL )   return "u32 did not convert correctly";
  
  n = Number::Int( i64 );
  if( n.AsInt() != -1LL )                   return "i64 did not convert correctly";
  if( n.AsFloat() != -1.0 )                 return "i64 -> float did not convert correctly";
  n = Number::Int( u64 );
  if( n.AsInt() != 0xffffffffffffffffULL )  return "u64 did not convert correctly";

  n = Number::Float( f32 );
  if( n.AsInt() != -1LL )                   return "f32 did not convert correctly";
  n = Number::Float( f64 );
  if( n.AsInt() != -1LL )                   return "f64 did not convert correctly";
  
  return NULL;
}

//--------------------------------------------------------------------------------------------------
