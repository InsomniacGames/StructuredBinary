//
//  TestAliasedPointer.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "TestAliasedPointer.h"

#include <string.h>
#include <stdio.h>

#include "sbSchema.h"
#include "sbAggregateType.h"
#include "sbAllocator.h"

namespace Src
{
  struct Struct
  {
    const char* string1;
    const char* string2;
    const char* string3;
    const char* string4;
  };
};

namespace Dst
{
  struct Struct
  {
    const char* string3;
    const char* string4;
    const char* string1;
    const char* string2;
  };
};

UnitTest::Result TestAliasedPointer::RunTest() const
{
  const char* src_stringA = "ABC";
  const char* src_stringB = "XYZ";
  
  Src::Struct src_struct =
  {
    src_stringA,  // const char* string1;
    src_stringA,  // const char* string2;
    src_stringB,  // const char* string3;
    src_stringB,  // const char* string4;
  };
  
  sbSchema dst_schema;
  dst_schema.Begin();

  dst_schema.BeginAggregate( "Struct" );
  dst_schema.AddStringPointer( "string1", 1, "int8_t", "value", sbScalarValue::Int( 0 ) );
  dst_schema.AddStringPointer( "string2", 1, "int8_t", "value", sbScalarValue::Int( 0 ) );
  dst_schema.AddStringPointer( "string3", 1, "int8_t", "value", sbScalarValue::Int( 0 ) );
  dst_schema.AddStringPointer( "string4", 1, "int8_t", "value", sbScalarValue::Int( 0 ) );
  dst_schema.EndAggregate();
  
  dst_schema.End();
  
  sbSchema src_schema;
  src_schema.Begin();

  src_schema.BeginAggregate( "StringElem" );
  src_schema.AddInstance( "c", 1, "int8_t" );
  src_schema.EndAggregate();
  
  src_schema.BeginAggregate( "Struct" );
  src_schema.AddStringPointer( "string3", 1, "int8_t", "value", sbScalarValue::Int( 0 ) );
  src_schema.AddStringPointer( "string4", 1, "int8_t", "value", sbScalarValue::Int( 0 ) );
  src_schema.AddStringPointer( "string1", 1, "int8_t", "value", sbScalarValue::Int( 0 ) );
  src_schema.AddStringPointer( "string2", 1, "int8_t", "value", sbScalarValue::Int( 0 ) );
  src_schema.EndAggregate();

  src_schema.End();
  
  sbAllocator alloc( NULL, 0 );
  dst_schema.Convert( ( const char* )&src_struct, &src_schema, "Struct", &alloc );
//  printf( "Memory needed %lu\n", alloc.GetSize() );

  char buffer[ 1000 ];
  alloc = sbAllocator( buffer, sizeof( buffer ) );
  Dst::Struct* dst_struct = ( Dst::Struct* )dst_schema.Convert( ( const char* )&src_struct, &src_schema, "Struct", &alloc );
  
  if( 0 != strcmp( dst_struct->string1, "ABC" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct->string1 );
  if( 0 != strcmp( dst_struct->string3, "XYZ" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct->string3 );

  if( dst_struct->string1 != dst_struct->string2 ) return Error( "Source aliased pointer is no longer aliased after conversion" );
  if( dst_struct->string3 != dst_struct->string4 ) return Error( "Source aliased pointer is no longer aliased after conversion" );

  return Ok();
}
