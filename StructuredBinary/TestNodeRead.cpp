//
//  TestNodeRead.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/3/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "TestNodeRead.h"

#include <stdint.h>
#include <stdio.h>

#include "sbSchema.h"
#include "sbNode.h"

namespace Src
{
  struct Node0
  {
    uint8_t   p;
    uint8_t   q;
  };
  
  struct Node1
  {
    uint8_t   a;
    uint8_t   count;
    Node0*    ptr;
    Node0*    ptr_array;
    float     c;
  };
  
  struct Node2
  {
    uint32_t  x;
    uint8_t   y;
    Node1     node1;
    uint32_t  z;
  };
};

UnitTestResult TestNodeRead::RunTest() const
{
  sbSchema schema;
  
  sbNode node0;
  node0.AddScalar( "P", 1, kScalar_U8 );
  node0.AddScalar( "Q", 1, kScalar_U8 );

  sbNode node1;
  node1.AddScalar( "A", 1, kScalar_U8 );
  node1.AddScalar( "COUNT", 1, kScalar_U8 );
  node1.AddPointer( "ptr", 1, "node0", NULL );
  node1.AddPointer( "ptr_array", 1, "node0", "COUNT" );
  node1.AddScalar( "C", 1, kScalar_F32 );

  sbNode node2;
  node2.AddScalar( "X", 1, kScalar_U32 );
  node2.AddScalar( "Y", 1, kScalar_U8 );
  node2.AddInstance( "inst", 1, "node1" );
  node2.AddScalar( "Z", 1, kScalar_U32 );

  schema.AddNode( "node0", &node0 );
  schema.AddNode( "node1", &node1 );
  schema.AddNode( "node2", &node2 );

  schema.FixUp();
  
  Src::Node0 n0 =
  {
    11,
    12
  };

  Src::Node0 array0[] =
  {
    {
      99,
      98
    },
    {
      97,
      96
    },
    {
      95,
      94
    },
    {
      93,
      92
    }
  };

  Src::Node2 n2 =
  {
    1,
    2,
    {
      3,
      4,
      &n0,
      array0,
      5.0f
    },
    6
  };

  printf( "n0=%p\n", &n0 );
  schema.PrintNode( "node2", ( const char* )&n2 );

  return UnitTestResult::Ok();
}
