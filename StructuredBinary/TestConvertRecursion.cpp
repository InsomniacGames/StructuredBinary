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
#include "sbAggregate.h"
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
  Src::Struct src_struct;
  Src::Struct src_and;

  src_struct.string = "ROUND";
  src_struct.next = &src_and;
  src_and.string = "AND";
  src_and.next = &src_struct;

  sbSchema src_schema;
  src_schema.Begin();
  src_schema.BeginElement( "Struct" );
  src_schema.AddString( "string", 1, "int8_t", "value", sbValue::Int( 0 ) );
  src_schema.AddPointer( "next", 1, "Struct", 0U );
  src_schema.EndElement();
  src_schema.End();
  
  sbSchema dst_schema;
  dst_schema.Begin();
  dst_schema.BeginElement( "Struct" );
  dst_schema.AddPointer( "next", 1, "Struct", 0U );
  dst_schema.AddString( "string", 1, "int8_t", "value", sbValue::Int( 0 ) );
  dst_schema.EndElement();
  dst_schema.End();

  sbAllocator alloc( NULL, 0 );
  dst_schema.Convert( NULL, ( const char* )&src_struct, &src_schema, "Struct", &alloc );
  //  printf( "Memory needed %lu\n", alloc.GetSize() );
  
  Dst::Struct dst_struct;
  char buffer[ 1000 ];
  alloc = sbAllocator( buffer, sizeof( buffer ) );
  dst_schema.Convert( ( char* )&dst_struct, ( const char* )&src_struct, &src_schema, "Struct", &alloc );

  if( 0 != strcmp( dst_struct.string, "ROUND" ) )             return Error( "1st string wrong" );
  if( 0 != strcmp( dst_struct.next->string, "AND" ) )         return Error( "2nd string wrong" );
  if( 0 != strcmp( dst_struct.next->next->string, "ROUND" ) ) return Error( "3nd string wrong" );

  return Ok();
}
