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
#include "sbNode.h"
#include "sbAllocator.h"

namespace Src
{
  struct InstElem
  {
    int32_t val;
  };

  struct ArrayElem
  {
    int32_t x;
    int32_t y;
    int32_t z;
  };

  struct StringElem
  {
    int8_t c;
  };
  
  struct Struct
  {
    InstElem    inst[ 2 ];
    uint32_t    array_count;
    ArrayElem*  array;
    StringElem* string;
  };
};

namespace Dst
{
  struct InstElem
  {
    int32_t val;
  };
  
  struct ArrayElem
  {
    int32_t z;
    int32_t x;
    int32_t y;
  };
  
  struct StringElem
  {
    int8_t c;
  };
  
  struct Struct
  {
    uint32_t    array_count;
    StringElem* string;
    ArrayElem*  array;
    InstElem    inst[ 2 ];
  };
};

UnitTest::Result TestConvertDeep::RunTest() const
{
  Src::ArrayElem src_array[ 2 ] =
  {
    { 10, 20, 30 },
    { 40, 50, 60 }
  };
  
  Src::StringElem src_string[] = { {'A'}, {'B'}, {'C'}, {0} };

  Src::Struct src_struct =
  {
    {
      { 1 },
      { 2 },
    },          // InstElem    inst[ 2 ]; 
    2,          // uint32_t    array_count;
    src_array,  // ArrayElem*  array;
    src_string  // StringElem* string;
  };

  sbSchema dst_schema;
  dst_schema.Begin();

  dst_schema.BeginNode( "InstElem" );
  dst_schema.AddScalar( "val", 1, kScalar_I32  );
  dst_schema.EndNode();

  dst_schema.BeginNode( "ArrayElem" );
  dst_schema.AddScalar( "z", 1, kScalar_I32  );
  dst_schema.AddScalar( "x", 1, kScalar_I32  );
  dst_schema.AddScalar( "y", 1, kScalar_I32  );
  dst_schema.EndNode();
  
  dst_schema.BeginNode( "StringElem" );
  dst_schema.AddScalar( "c", 1, kScalar_I8  );
  dst_schema.EndNode();
  
  dst_schema.BeginNode( "Struct" );
  dst_schema.AddScalar  ( "array_count", 1, kScalar_I32 );
  dst_schema.AddString  ( "string", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  dst_schema.AddPointer ( "array", 1, "ArrayElem", "array_count" );
  dst_schema.AddInstance( "inst", 2, "InstElem" );
  dst_schema.EndNode();

  dst_schema.End();

  sbSchema src_schema;
  src_schema.Begin();

  src_schema.BeginNode( "InstElem" );
  src_schema.AddScalar( "val", 1, kScalar_I32  );
  src_schema.EndNode();
  
  src_schema.BeginNode( "ArrayElem" );
  src_schema.AddScalar( "x", 1, kScalar_I32  );
  src_schema.AddScalar( "y", 1, kScalar_I32  );
  src_schema.AddScalar( "z", 1, kScalar_I32  );
  src_schema.EndNode();
  
  src_schema.BeginNode( "StringElem" );
  src_schema.AddScalar( "c", 1, kScalar_I8  );
  src_schema.EndNode();
  
  src_schema.BeginNode( "Struct" );
  src_schema.AddInstance( "inst", 2, "InstElem" );
  src_schema.AddScalar  ( "array_count", 1, kScalar_I32 );
  src_schema.AddPointer ( "array", 1, "ArrayElem", "array_count" );
  src_schema.AddString  ( "string", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  src_schema.EndNode();
  
  src_schema.End();

  sbAllocator alloc( NULL, 0 );
  dst_schema.Convert( NULL, ( const char* )&src_struct, &src_schema, "Struct", &alloc );
  printf( "Memory needed %lu\n", alloc.GetSize() );

  Dst::Struct dst_struct;
  char buffer[ 1000 ];
  alloc = sbAllocator( buffer, sizeof( buffer ) );
  dst_schema.Convert( ( char* )&dst_struct, ( const char* )&src_struct, &src_schema, "Struct", &alloc );

  if( dst_struct.array_count   != 2 )           return Error( "array_count wrong value" );
  if( dst_struct.array == NULL )                return Error( "array is NULL" );
  if( dst_struct.array[ 0 ].x  != 10 )          return Error( "array[ 0 ].x wrong value" );
  if( dst_struct.array[ 0 ].y  != 20 )          return Error( "array[ 0 ].y wrong value" );
  if( dst_struct.array[ 0 ].z  != 30 )          return Error( "array[ 0 ].z wrong value" );
  if( dst_struct.array[ 1 ].x  != 40 )          return Error( "array[ 1 ].x wrong value" );
  if( dst_struct.array[ 1 ].y  != 50 )          return Error( "array[ 1 ].y wrong value" );
  if( dst_struct.array[ 1 ].z  != 60 )          return Error( "array[ 1 ].z wrong value" );
  if( dst_struct.string == NULL )               return Error( "string is NULL" );
  if( 0 != strcmp( ( const char* )dst_struct.string, "ABC" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct.string );
  if( dst_struct.inst[ 0 ].val != 1 )           return Error( "inst[ 0 ] wrong value" );
  if( dst_struct.inst[ 1 ].val != 2 )           return Error( "inst[ 1 ] wrong value" );
  
  return Ok();
}
