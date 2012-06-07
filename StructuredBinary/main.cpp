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
#include "TestNodeRead.h"

const UnitTest* AllTests[] =
{
  new TestNodeRead(),
};

int main (int argc, const char * argv[])
{
  int test_count = ARRAY_SIZE( AllTests );
  printf( "test_count = %d\n", test_count );
  const UnitTest** test_array = AllTests;

  int fail_count = 0;
  
  for( int i = 0; i < test_count; ++i )
  {
    const UnitTest* test = test_array[ i ];
    printf( "[%2d] Testing %s\n", i, test->GetName() );
    UnitTestResult result = test->Run();
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
