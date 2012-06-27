//
//  TestConvertDeep.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/10/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "TestConvertDeep.h"

#include <string.h>
#include <stdio.h>

#include "sbSchema.h"
#include "sbAggregateType.h"
#include "sbAllocator.h"

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
    uint32_t    count;
    PointerElem*  pointer;
    const char* string[3];
    float f[2];
  };
};

namespace Dst
{
  struct PointerElem
  {
    int32_t z;
    int32_t x;
    int32_t y;
  };

  struct Struct
  {
    float f[2];
    uint32_t    count;
    const char* string[3];
    PointerElem*  pointer;
  };
};

UnitTest::Result TestConvertDeep::RunTest() const
{
  Src::PointerElem src_array[ 2 ] =
  {
    { 10, 20, 30 },
    { 40, 50, 60 }
  };
  
  Src::Struct src_struct =
  {
    2,          // uint32_t    count;
    src_array,  // PointerElem*  pointer;
    {
      "zero",
      "one",
      "two"
    },
    {
      100.0f,
      200.0f
    }
  };

  sbSchema dst_schema;
  dst_schema.Begin();

  dst_schema.BeginAggregate( "PointerElem" );
  dst_schema.AddInstance( "z", 1, "int32_t" );
  dst_schema.AddInstance( "x", 1, "int32_t" );
  dst_schema.AddInstance( "y", 1, "int32_t" );
  dst_schema.EndAggregate();
    
  dst_schema.BeginAggregate( "Struct" );
  dst_schema.AddInstance  ( "f", 2, "float" );
  dst_schema.AddInstance  ( "count", 1, "int32_t" );
  dst_schema.AddStringPointer  ( "string", 3, "int8_t", "value", sbScalarValue::Int( 0 ) );
  dst_schema.AddCountedPointer ( "pointer", 1, "PointerElem", "count" );
  dst_schema.EndAggregate();

  dst_schema.End();

  sbSchema src_schema;
  src_schema.Begin();
  
  src_schema.BeginAggregate( "PointerElem" );
  src_schema.AddInstance( "x", 1, "int32_t" );
  src_schema.AddInstance( "y", 1, "int32_t" );
  src_schema.AddInstance( "z", 1, "int32_t" );
  src_schema.EndAggregate();

  src_schema.BeginAggregate( "Struct" );
  src_schema.AddInstance  ( "count", 1, "int32_t" );
  src_schema.AddCountedPointer ( "pointer", 1, "PointerElem", "count" );
  src_schema.AddStringPointer  ( "string", 3, "int8_t", "value", sbScalarValue::Int( 0 ) );
  src_schema.AddInstance  ( "f", 2, "float" );
  src_schema.EndAggregate();
  
  src_schema.End();

  sbAllocator alloc( NULL, 0 );
  dst_schema.Convert( ( const char* )&src_struct, &src_schema, "Struct", &alloc );
  printf( "Memory needed %lu\n", alloc.GetSize() );
  printf( "Pointers found: %d\n", alloc.GetPointerLocationCount() );

  char buffer[ 1000 ];
  alloc = sbAllocator( buffer, sizeof( buffer ) );
  Dst::Struct* dst_struct = ( Dst::Struct* )dst_schema.Convert( ( const char* )&src_struct, &src_schema, "Struct", &alloc );

  if( dst_struct->count   != 2 )           return Error( "count wrong value" );
  if( dst_struct->pointer == NULL )        return Error( "pointer is NULL" );
  if( dst_struct->pointer[ 0 ].x  != 10 )  return Error( "pointer[ 0 ].x wrong value" );
  if( dst_struct->pointer[ 0 ].y  != 20 )  return Error( "pointer[ 0 ].y wrong value" );
  if( dst_struct->pointer[ 0 ].z  != 30 )  return Error( "pointer[ 0 ].z wrong value" );
  if( dst_struct->pointer[ 1 ].x  != 40 )  return Error( "pointer[ 1 ].x wrong value" );
  if( dst_struct->pointer[ 1 ].y  != 50 )  return Error( "pointer[ 1 ].y wrong value" );
  if( dst_struct->pointer[ 1 ].z  != 60 )  return Error( "pointer[ 1 ].z wrong value" );
  if( 0 != strcmp( ( const char* )dst_struct->string[0], "zero" ) ) return Error( "string[0] wrong value: %s", ( const char* )dst_struct->string[0] );
  if( 0 != strcmp( ( const char* )dst_struct->string[1], "one" ) ) return Error( "string[1] wrong value: %s", ( const char* )dst_struct->string[1] );
  if( 0 != strcmp( ( const char* )dst_struct->string[2], "two" ) ) return Error( "string[2] wrong value: %s", ( const char* )dst_struct->string[2] );
  if( dst_struct->f[ 0 ] != 100.0f )       return Error( "f[0] wrong value" );
  if( dst_struct->f[ 1 ] != 200.0f )       return Error( "f[1] wrong value" );

//  printf( "%s %s %s\n", dst_struct.string[ 0 ], dst_struct.string[ 1 ], dst_struct.string[ 2 ] );

  return Ok();
}
