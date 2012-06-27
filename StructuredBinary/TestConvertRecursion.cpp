//
//  TestConvertRecursion.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "TestConvertRecursion.h"

#include <string.h>
#include <stdio.h>

#include "sbSchema.h"
#include "sbAggregateType.h"
#include "sbAllocator.h"

namespace Src
{
  struct Struct
  {
    const char* string;
    Struct*     next;
  };
};

namespace Dst
{
  struct Struct
  {
    Struct*     next;
    const char* string;
  };
};

UnitTest::Result TestConvertRecursion::RunTest() const
{
  Src::Struct src_round;
  Src::Struct src_and;

  src_round.string = "ROUND";
  src_round.next = &src_and;
  src_and.string = "AND";
  src_and.next = &src_round;

  sbSchema src_schema;
  src_schema.Begin();
  src_schema.BeginAggregate( "Struct" );
  src_schema.AddStringPointer( "string", 1, "int8_t", "value", sbScalarValue::Int( 0 ) );
  src_schema.AddPointer( "next", 1, "Struct" );
  src_schema.EndAggregate();
  src_schema.End();
  
  sbSchema dst_schema;
  dst_schema.Begin();
  dst_schema.BeginAggregate( "Struct" );
  dst_schema.AddPointer( "next", 1, "Struct" );
  dst_schema.AddStringPointer( "string", 1, "int8_t", "value", sbScalarValue::Int( 0 ) );
  dst_schema.EndAggregate();
  dst_schema.End();

  sbAllocator alloc( NULL, 0 );
  dst_schema.Convert( ( const char* )&src_round, &src_schema, "Struct", &alloc );
  //  printf( "Memory needed %lu\n", alloc.GetSize() );
  
  char buffer[ 1000 ];
  alloc = sbAllocator( buffer, sizeof( buffer ) );
  Dst::Struct* dst_struct = ( Dst::Struct* )dst_schema.Convert( ( const char* )&src_round, &src_schema, "Struct", &alloc );

  if( 0 != strcmp( dst_struct->string, "ROUND" ) )             return Error( "1st string wrong" );
  if( 0 != strcmp( dst_struct->next->string, "AND" ) )         return Error( "2nd string wrong" );
  if( 0 != strcmp( dst_struct->next->next->string, "ROUND" ) ) return Error( "3nd string wrong" );

  return Ok();
}
