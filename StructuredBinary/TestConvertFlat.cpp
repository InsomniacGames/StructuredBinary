//
//  TestConvertFlat.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/9/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "TestConvertFlat.h"

#include "sbSchema.h"
#include "sbNode.h"

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
  sbNode src_node;
  src_node.AddScalar( "u8" , 1, kScalar_U8  );
  src_node.AddScalar( "f32", 1, kScalar_F32 );
  src_node.AddScalar( "u16", 1, kScalar_U16 );
  src_node.AddScalar( "u32", 1, kScalar_U32 );
  src_node.AddScalar( "u64", 1, kScalar_U64 );
  src_node.AddScalar( "i8" , 1, kScalar_I8  );
  src_node.AddScalar( "f64", 1, kScalar_F64 );
  src_node.AddScalar( "i16", 1, kScalar_I16 );
  src_node.AddScalar( "i32", 1, kScalar_I32 );
  src_node.AddScalar( "i64", 1, kScalar_I64 );
  sbSchema src_schema;
  src_schema.AddNode( "Struct", &src_node );
  src_schema.FixUp();

  sbNode dst_node;
  dst_node.AddScalar( "i8" , 1, kScalar_I8  );
  dst_node.AddScalar( "f64", 1, kScalar_F64 );
  dst_node.AddScalar( "i16", 1, kScalar_I16 );
  dst_node.AddScalar( "i32", 1, kScalar_I32 );
  dst_node.AddScalar( "i64", 1, kScalar_I64 );
  dst_node.AddScalar( "u8" , 1, kScalar_U8  );
  dst_node.AddScalar( "f32", 1, kScalar_F32 );
  dst_node.AddScalar( "u16", 1, kScalar_U16 );
  dst_node.AddScalar( "u32", 1, kScalar_U32 );
  dst_node.AddScalar( "u64", 1, kScalar_U64 );
  sbSchema dst_schema;
  dst_schema.AddNode( "Struct", &dst_node );
  dst_schema.FixUp();

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

  Dst::Struct dst_struct;

  dst_schema.Convert( ( char* )&dst_struct, ( const char* )&src_struct, &src_schema, "Struct" );

  if( dst_struct.u8   != 200 )                  return Error( "u8 wrong value" );
  if( dst_struct.f32  != 3.141f )               return Error( "f32 wrong value" );
  if( dst_struct.u16  != 60000 )                return Error( "u16 wrong value" );
  if( dst_struct.u32  != 12345 )                return Error( "u32 wrong value" );
  if( dst_struct.u64  != 0xde53a0914a8420efLL ) return Error( "u64 wrong value" );
  if( dst_struct.i8   != -100 )                 return Error( "i8 wrong value" );
  if( dst_struct.f64  != 1.23456789 )           return Error( "f64 wrong value" );
  if( dst_struct.i16  != -30 )                  return Error( "i16 wrong value" );
  if( dst_struct.i32  != 0x23eda091 )           return Error( "i32 wrong value" );
  if( dst_struct.i64  != 0x193d59739a0ca2c1LL ) return Error( "164 wrong value" );

  return Ok();
}
