//
//  TestSubStructConvert.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/15/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "TestSubStructConvert.h"

// Libraries
#include <cstring>
#include <stdint.h>

// Project
#include "Number.h"
#include "Field.h"
#include "Aggregate.h"

//--------------------------------------------------------------------------------------------------

struct SourceStruct
{
  float   var1;
  float   var2;
  struct
  {
    float var1;
    float var2;
  } sub1;
  struct
  {
    float var1;
    float var2;
  } sub2;
};

struct DestStruct
{
  float   var2;
  struct
  {
    float var2;
    float var1;
  } sub1;
  struct
  {
    float var1;
    float var2;
  } sub2;
  float   var1;
};

//--------------------------------------------------------------------------------------------------

const char* TestSubStructConvert::RunTest() const
{
  Aggregate src_sub1( 2 );
  src_sub1.SetFloat32 ( 0,  0, "var1" );
  src_sub1.SetFloat32 ( 1,  4, "var2" );
  Aggregate src_sub2( 2 );
  src_sub2.SetFloat32 ( 0,  0, "var1" );
  src_sub2.SetFloat32 ( 1,  4, "var2" );
  Aggregate src_agg( 4 );
  src_agg.SetFloat32  ( 0,  0, "var1" );
  src_agg.SetFloat32  ( 1,  4, "var2" );
  src_agg.SetSubStruct( 2,  8, "sub1", &src_sub1 );
  src_agg.SetSubStruct( 3, 16, "sub2", &src_sub2 );
  
  Aggregate dst_sub1( 2 );
  dst_sub1.SetFloat32 ( 0,  0, "var2" );
  dst_sub1.SetFloat32 ( 1,  4, "var1" );
  Aggregate dst_sub2( 2 );
  dst_sub2.SetFloat32 ( 0,  0, "var1" );
  dst_sub2.SetFloat32 ( 1,  4, "var2" );
  Aggregate dst_agg( 4 );
  dst_agg.SetFloat32  ( 0,  0, "var2" );
  dst_agg.SetSubStruct( 1,  4, "sub1", &dst_sub1 );
  dst_agg.SetSubStruct( 2, 12, "sub2", &dst_sub2 );
  dst_agg.SetFloat32  ( 3, 20, "var1" );
  
  SourceStruct src;
  src.var1      = 1.0f;
  src.var2      = 2.0f;
  src.sub1.var1 = 3.0f;
  src.sub1.var2 = 4.0f;
  src.sub2.var1 = 5.0f;
  src.sub2.var2 = 6.0f;
  
  DestStruct dst;
  
  const char* src_data = ( const char* )&src;
  char* dst_data = ( char* )&dst;
  
  dst_agg.Convert( dst_data, src_data, &src_agg );

  if( dst.var1      != src.var1 )       return "var1 is wrong";
  if( dst.var2      != src.var2 )       return "var2 is wrong";
  if( dst.sub1.var1 != src.sub1.var1 )  return "sub1.var1 is wrong";
  if( dst.sub1.var2 != src.sub1.var2 )  return "sub1.var2 is wrong";
  if( dst.sub2.var1 != src.sub2.var1 )  return "sub2.var1 is wrong";
  if( dst.sub2.var2 != src.sub2.var2 )  return "sub2.var2 is wrong";

  return NULL;
}
