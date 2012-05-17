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
#include "Util.h"
#include "UnitTestNumberConvert.h"
#include "UnitTestScalarConvert.h"
#include "UnitTestSubStructConvert.h"
#include "UnitTestChunk.h"
#include "UnitTestScalarRead.h"

const UnitTest* AllTests[] =
{
  new UnitTestScalarRead(),
  new UnitTestNumberConvert(),
  new UnitTestScalarConvert(),
  new UnitTestSubStructConvert(),
  new UnitTestChunk(),
};

int main (int argc, const char * argv[])
{
  int test_count = ARRAY_SIZE( AllTests );
  const UnitTest** test_array = AllTests;

  int fail_count = 0;
  
  for( int i = 0; i < test_count; ++i )
  {
    const UnitTest* test = test_array[ i ];
    printf( "Testing %s\n", test->GetName() );
    const char* error_message = test->RunTest();
    if( error_message )
    {
      printf( "** FAIL: %s\n", error_message );
      fail_count += 1;
    }
  }
  
  printf( "%d test%s failed\n", fail_count, fail_count != 1 ? "s" : "" );
  return fail_count;

}
