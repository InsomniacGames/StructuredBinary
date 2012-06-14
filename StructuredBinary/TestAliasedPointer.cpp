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
#include "sbNode.h"
#include "sbAllocator.h"

namespace Src
{
  struct StringElem
  {
    int8_t c;
  };
  
  struct Struct
  {
    StringElem* string1;
    StringElem* string2;
    StringElem* string3;
    StringElem* string4;
  };
};

namespace Dst
{
  struct StringElem
  {
    int8_t c;
  };
  
  struct Struct
  {
    StringElem* string3;
    StringElem* string4;
    StringElem* string1;
    StringElem* string2;
  };
};

UnitTest::Result TestAliasedPointer::RunTest() const
{
  Src::StringElem src_stringA[] = { {'A'}, {'a'}, {'a'}, {0} };
  Src::StringElem src_stringB[] = { {'B'}, {'b'}, {'b'}, {0} };
  
  Src::Struct src_struct =
  {
    src_stringA,  // StringElem* string1;
    src_stringA,  // StringElem* string2;
    src_stringB,  // StringElem* string3;
    src_stringB,  // StringElem* string4;
  };
  
  sbSchema dst_schema;
  dst_schema.Begin();

  dst_schema.BeginNode( "StringElem" );
  dst_schema.AddScalar( "c", 1, kScalar_I8  );
  dst_schema.EndNode();

  dst_schema.BeginNode( "Struct" );
  dst_schema.AddString( "string1", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  dst_schema.AddString( "string2", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  dst_schema.AddString( "string3", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  dst_schema.AddString( "string4", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  dst_schema.EndNode();
  
  dst_schema.End();
  
  sbSchema src_schema;
  src_schema.Begin();

  src_schema.BeginNode( "StringElem" );
  src_schema.AddScalar( "c", 1, kScalar_I8  );
  src_schema.EndNode();
  
  src_schema.BeginNode( "Struct" );
  src_schema.AddString( "string3", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  src_schema.AddString( "string4", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  src_schema.AddString( "string1", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  src_schema.AddString( "string2", 1, "StringElem", sbScalarValue::Int( 0 ), "c" );
  src_schema.EndNode();

  src_schema.End();
  
  sbAllocator alloc( NULL, 0 );
  dst_schema.Convert( NULL, ( const char* )&src_struct, &src_schema, "Struct", &alloc );
  printf( "Memory needed %lu\n", alloc.GetSize() );
  
  Dst::Struct dst_struct;
  char buffer[ 1000 ];
  alloc = sbAllocator( buffer, sizeof( buffer ) );
  dst_schema.Convert( ( char* )&dst_struct, ( const char* )&src_struct, &src_schema, "Struct", &alloc );
  
  if( 0 != strcmp( ( const char* )dst_struct.string1, "Aaa" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct.string1 );
  if( 0 != strcmp( ( const char* )dst_struct.string2, "Aaa" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct.string2 );
  if( 0 != strcmp( ( const char* )dst_struct.string3, "Bbb" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct.string3 );
  if( 0 != strcmp( ( const char* )dst_struct.string4, "Bbb" ) ) return Error( "string wrong value: %s", ( const char* )dst_struct.string4 );

  if( dst_struct.string1 != dst_struct.string2 ) return Error( "Source aliased pointer is no longer aliased after conversion" );
  if( dst_struct.string3 != dst_struct.string4 ) return Error( "Source aliased pointer is no longer aliased after conversion" );

  return Ok();
}
