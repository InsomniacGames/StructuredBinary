//
//  TestConvertScalarTypes.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "TestScalarTypes.h"

#include "sbSchema.h"
#include "sbAggregateType.h"

namespace Src
{
  struct Struct
  {
    uint8_t   u8_to_i64;
    int8_t    i8_to_i64;
    uint8_t   u8_to_f64;
    int8_t    i8_to_f64;
    double    f64_to_i32;
  };
};

namespace Dst
{
  struct Struct
  {
    int64_t  u8_to_i64;
    int64_t  i8_to_i64;
    double   u8_to_f64;
    double   i8_to_f64;
    int32_t  f64_to_i32;
  };
};

UnitTest::Result TestScalarTypes::RunTest() const
{
  sbSchema src_schema;
  src_schema.Begin();
  
  src_schema.BeginAggregate( "Struct" );
  src_schema.AddInstance( "u8_to_i64" , 1, "uint8_t");
  src_schema.AddInstance( "i8_to_i64" , 1, "int8_t"  );
  src_schema.AddInstance( "u8_to_f64" , 1, "uint8_t");
  src_schema.AddInstance( "i8_to_f64" , 1, "int8_t"  );
  src_schema.AddInstance( "f64_to_i32" , 1, "double"  );
  src_schema.EndAggregate();
  
  src_schema.End();
  
  sbSchema dst_schema;
  dst_schema.Begin();
  
  dst_schema.BeginAggregate( "Struct" );
  dst_schema.AddInstance( "u8_to_i64" , 1, "int64_t" );
  dst_schema.AddInstance( "i8_to_i64" , 1, "int64_t" );
  dst_schema.AddInstance( "u8_to_f64" , 1, "double" );
  dst_schema.AddInstance( "i8_to_f64" , 1, "double" );
  dst_schema.AddInstance( "f64_to_i32" , 1, "int32_t"  );
  dst_schema.EndAggregate();
  
  dst_schema.End();
  
  Src::Struct src_struct;
  src_struct.u8_to_i64   =  200;
  src_struct.i8_to_i64   = -100;
  src_struct.u8_to_f64   =  200;
  src_struct.i8_to_f64   = -100;
  src_struct.f64_to_i32  = -1234567890.0;

  char buffer[ 1000 ];
  sbAllocator alloc = sbAllocator( buffer, sizeof( buffer ) );

  Dst::Struct* dst_struct = ( Dst::Struct* )dst_schema.Convert( ( const char* )&src_struct, &src_schema, "Struct", &alloc );
  
  if( dst_struct->u8_to_i64 !=  200 )                  return Error( "u8_to_i64 wrong value" );
  if( dst_struct->i8_to_i64 != -100 )                  return Error( "i8_to_i64 wrong value" );
  if( dst_struct->u8_to_f64 !=  200.0 )                return Error( "u8_to_f64 wrong value" );
  if( dst_struct->i8_to_f64 != -100.0 )                return Error( "i8_to_f64 wrong value" );
  if( dst_struct->f64_to_i32 != -1234567890 )          return Error( "f64_to_i32 wrong value" );

  return Ok();
}
