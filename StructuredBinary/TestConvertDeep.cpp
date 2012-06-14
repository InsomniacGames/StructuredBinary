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

  sbNode dst_inst_elem_node;
  dst_inst_elem_node.AddScalar( "val", 1, kScalar_I32  );
  
  sbNode dst_array_elem_node;
  dst_array_elem_node.AddScalar( "z", 1, kScalar_I32  );
  dst_array_elem_node.AddScalar( "x", 1, kScalar_I32  );
  dst_array_elem_node.AddScalar( "y", 1, kScalar_I32  );
  
  sbNode dst_string_elem_node;
  dst_string_elem_node.AddScalar( "c", 1, kScalar_I8  );
  
  sbNode dst_struct_node;
  dst_struct_node.AddScalar  ( "array_count", 1, kScalar_I32 );
  dst_struct_node.AddString  ( "string", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  dst_struct_node.AddPointer ( "array", 1, "ArrayElem", "array_count" );
  dst_struct_node.AddInstance( "inst", 2, "InstElem" );
  
  sbSchema dst_schema;
  dst_schema.AddNode( "InstElem", &dst_inst_elem_node );
  dst_schema.AddNode( "ArrayElem", &dst_array_elem_node );
  dst_schema.AddNode( "StringElem", &dst_string_elem_node );
  dst_schema.AddNode( "Struct", &dst_struct_node );
  dst_schema.FixUp();

  sbNode src_inst_elem_node;
  src_inst_elem_node.AddScalar( "val", 1, kScalar_I32  );
  
  sbNode src_array_elem_node;
  src_array_elem_node.AddScalar( "x", 1, kScalar_I32  );
  src_array_elem_node.AddScalar( "y", 1, kScalar_I32  );
  src_array_elem_node.AddScalar( "z", 1, kScalar_I32  );
  
  sbNode src_string_elem_node;
  src_string_elem_node.AddScalar( "c", 1, kScalar_I8  );
  
  sbNode src_struct_node;
  src_struct_node.AddInstance( "inst", 2, "InstElem" );
  src_struct_node.AddScalar  ( "array_count", 1, kScalar_I32 );
  src_struct_node.AddPointer ( "array", 1, "ArrayElem", "array_count" );
  src_struct_node.AddString  ( "string", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  
  sbSchema src_schema;
  src_schema.AddNode( "InstElem", &src_inst_elem_node );
  src_schema.AddNode( "ArrayElem", &src_array_elem_node );
  src_schema.AddNode( "StringElem", &src_string_elem_node );
  src_schema.AddNode( "Struct", &src_struct_node );
  src_schema.FixUp();

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
