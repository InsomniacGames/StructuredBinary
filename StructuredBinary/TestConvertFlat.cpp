//
//  TestConvertFlat.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/9/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "TestConvertFlat.h"

#include "sbSchema.h"
#include "sbAggregateType.h"

namespace Src
{
  // Alignment jumbled up on purpose!
  struct Struct
  {
    uint8_t   u8;
    float     f32;
    uint16_t  u16;
    uint32_t  u32;
    uint64_t  u64;
    int8_t    i8;
    double    f64;
    int16_t   i16;
    int32_t   i32;
    int64_t   i64;
  };
};

namespace Dst
{
  // Alignment jumbled up on purpose!
  struct Struct
  {
    int8_t    i8;
    double    f64;
    int16_t   i16;
    int32_t   i32;
    int64_t   i64;
    uint8_t   u8;
    float     f32;
    uint16_t  u16;
    uint32_t  u32;
    uint64_t  u64;
  };
};

UnitTest::Result TestConvertFlat::RunTest() const
{
  sbSchema src_schema;
  src_schema.Begin();

  src_schema.BeginAggregate( "Struct" );
  src_schema.AddInstance( "u8" , 1, "uint8_t");
  src_schema.AddInstance( "f32", 1, "float" );
  src_schema.AddInstance( "u16", 1, "uint16_t" );
  src_schema.AddInstance( "u32", 1, "uint32_t" );
  src_schema.AddInstance( "u64", 1, "uint64_t" );
  src_schema.AddInstance( "i8" , 1, "int8_t"  );
  src_schema.AddInstance( "f64", 1, "double" );
  src_schema.AddInstance( "i16", 1, "int16_t" );
  src_schema.AddInstance( "i32", 1, "int32_t" );
  src_schema.AddInstance( "i64", 1, "int64_t" );
  src_schema.EndAggregate();

  src_schema.End();

  sbSchema dst_schema;
  dst_schema.Begin();

  dst_schema.BeginAggregate( "Struct" );
  dst_schema.AddInstance( "i8" , 1, "int8_t" );
  dst_schema.AddInstance( "f64", 1, "double" );
  dst_schema.AddInstance( "i16", 1, "int16_t" );
  dst_schema.AddInstance( "i32", 1, "int32_t" );
  dst_schema.AddInstance( "i64", 1, "int64_t" );
  dst_schema.AddInstance( "u8" , 1, "uint8_t" );
  dst_schema.AddInstance( "f32", 1, "float" );
  dst_schema.AddInstance( "u16", 1, "uint16_t" );
  dst_schema.AddInstance( "u32", 1, "uint32_t" );
  dst_schema.AddInstance( "u64", 1, "uint64_t" );
  dst_schema.EndAggregate();

  dst_schema.End();

  Src::Struct src_struct;
  src_struct.u8   = 200;
  src_struct.f32  = 3.141f;
  src_struct.u16  = 60000;
  src_struct.u32  = 12345;
  src_struct.u64  = 0xde53a0914a8420efULL;
  src_struct.i8   = -100;
  src_struct.f64  = 1.23456789;
  src_struct.i16  = -30;
  src_struct.i32  = 0x23eda091;
  src_struct.i64  = 0x193d59739a0ca2c1LL;

  char buffer[ 1000 ];
  sbAllocator alloc = sbAllocator( buffer, sizeof( buffer ) );

  Dst::Struct* dst_struct = ( Dst::Struct* )dst_schema.Convert( ( const char* )&src_struct, &src_schema, "Struct", &alloc );

  if( dst_struct->u8   != 200 )                  return Error( "u8 wrong value" );
  if( dst_struct->f32  != 3.141f )               return Error( "f32 wrong value" );
  if( dst_struct->u16  != 60000 )                return Error( "u16 wrong value" );
  if( dst_struct->u32  != 12345 )                return Error( "u32 wrong value" );
  if( dst_struct->u64  != 0xde53a0914a8420efLL ) return Error( "u64 wrong value" );
  if( dst_struct->i8   != -100 )                 return Error( "i8 wrong value" );
  if( dst_struct->f64  != 1.23456789 )           return Error( "f64 wrong value" );
  if( dst_struct->i16  != -30 )                  return Error( "i16 wrong value" );
  if( dst_struct->i32  != 0x23eda091 )           return Error( "i32 wrong value" );
  if( dst_struct->i64  != 0x193d59739a0ca2c1LL ) return Error( "164 wrong value" );

  return Ok();
}
