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
#include "Test.h"
#include "Util.h"
#include "TestAlwaysSucceed.h"
#include "TestScalarRead.h"
#include "TestNumberConvert.h"

const Test* AllTests[] =
{
  new TestAlwaysSucceed(),
  new TestScalarRead(),
  new TestNumberConvert(),
};

int main (int argc, const char * argv[])
{
  int test_count = ARRAY_SIZE( AllTests );
  const Test** test_array = AllTests;

  int fail_count = 0;
  
  for( int i = 0; i < test_count; ++i )
  {
    const Test* test = test_array[ i ];
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
