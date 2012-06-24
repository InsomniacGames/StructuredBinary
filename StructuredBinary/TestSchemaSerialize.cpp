//
//  TestSchemaSerialize.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/23/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "TestSchemaSerialize.h"



#include <string.h>
#include <stdio.h>

#include "sbSchema.h"
#include "sbAggregateType.h"
#include "sbByteWriter.h"
#include "sbByteReader.h"

namespace Src
{
  struct PointerElem
  {
    int32_t x;
    int32_t y;
    int32_t z;
  };
  
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

    uint32_t    count;
    PointerElem*  pointer;
    const char* string[3];
    float f[2];
  };
};

UnitTest::Result TestSchemaSerialize::RunTest() const
{
  sbSchema src_schema;
  src_schema.Begin();
  
  src_schema.BeginAggregate( "PointerElem" );
  src_schema.AddInstance( "x", 1, "int32_t" );
  src_schema.AddInstance( "y", 1, "int32_t" );
  src_schema.AddInstance( "z", 1, "int32_t" );
  src_schema.EndAggregate();
  
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

  src_schema.AddInstance  ( "count", 1, "int32_t" );
  src_schema.AddCountedPointer ( "pointer", 1, "PointerElem", "count" );
  src_schema.AddStringPointer  ( "string", 3, "int8_t", "value", sbScalarValue::Int( 0 ) );
  src_schema.AddInstance  ( "f", 2, "float" );
  src_schema.EndAggregate();
  
  src_schema.End();

  uint64_t original_checksum = src_schema.GetChecksum();

  sbByteWriter writer;
  writer.Write( &src_schema );

  size_t calculated_size = writer.Tell();

  char* buffer = new char[ calculated_size ];
  writer.SetBuffer( buffer, calculated_size );

  writer.Write( &src_schema );
  size_t written_size = writer.Tell();

  sbByteReader reader( buffer, calculated_size );

  sbSchema* dup_src_schema = reader.ReadNew< sbSchema >();
  if( !dup_src_schema )                           return Error( "schema could not be read" );
  size_t read_size = writer.Tell();
  uint64_t duplicated_checksum = dup_src_schema->GetChecksum();
  
  if( calculated_size == 0 )                      return Error( "calculated size was zero" );
  if( calculated_size != written_size )           return Error( "calculated size was %lu but written size was %lu", calculated_size, written_size );
  if( calculated_size != read_size )              return Error( "calculated size was %lu but read size was %lu", calculated_size, read_size );
  if( original_checksum == 0 )                    return Error( "calculated checksum was zero" );
  if( original_checksum != duplicated_checksum )  return Error( "original and duplicated checksums do not match: %016llx vs  %016llx", original_checksum, duplicated_checksum );

  delete dup_src_schema;
  delete buffer;

  return Ok();
}
