//
//  main.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Libraries
#include <cstring>
#include <stdio.h>

// Project
#include "sbUtil.h"
#include "TestChunk.h"
#include "TestChunkFile.h"
#include "TestFormatFile.h"
#include "TestFormatFile2.h"
//#include "TestNodeRead.h"
#include "TestConvertFlat.h"
#include "TestConvertDeep.h"
#include "TestAliasedPointer.h"

#include "sbFnv.h"

const UnitTest* AllTests[] =
{
  new TestChunk(),
  new TestChunkFile(),
  new TestFormatFile(),
  new TestFormatFile2(),
//  new TestNodeRead(),
  new TestConvertFlat(),
  new TestConvertDeep(),
  new TestAliasedPointer(),
};

const char* names[] =
{
  "uint8_t",
  "int8_t",
  "uint16_t",
  "int16_t",
  "uint32_t",
  "int32_t",
  "uint64_t",
  "int64_t",
  "float",
  "double",
};

int main (int argc, const char * argv[])
{
  for( int i = 0; i < ARRAY_SIZE( names ); ++i )
  {
    const char* p = names[ i ];
    printf( "%8s:%08x\n", p, sbFnv32( p ) );
  }

  int test_count = ARRAY_SIZE( AllTests );
  printf( "test_count = %d\n", test_count );
  const UnitTest** test_array = AllTests;

  int fail_count = 0;
  
  for( int i = 0; i < test_count; ++i )
  {
    const UnitTest* test = test_array[ i ];
    printf( "[%2d] Testing %s\n", i, test->GetName() );
    UnitTest::Result result = test->Run();
    if( !result.IsSuccess() )
    {
      printf( "** FAIL: %s\n", result.GetMessage() );
      fail_count += 1;
    }
  }

  if( fail_count == 0 )
    printf( "All tests successful\n" );
  else
    printf( "%d test%s failed\n", fail_count, fail_count != 1 ? "s" : "" );
  return fail_count;

}
