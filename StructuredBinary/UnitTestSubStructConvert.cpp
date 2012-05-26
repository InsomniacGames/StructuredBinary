//
//  UnitTestSubStructConvert.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/15/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "UnitTestSubStructConvert.h"

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

const char* UnitTestSubStructConvert::RunTest() const
{
  Aggregate* src_sub1 = new Aggregate( 2 );
  src_sub1->AddField( Fnv32( "var1" ), kField_F32 );
  src_sub1->AddField( Fnv32( "var2" ), kField_F32 );
  src_sub1->FixSizeAndStride();

  Aggregate* src_sub2 = new Aggregate( 2 );
  src_sub2->AddField( Fnv32( "var1" ), kField_F32 );
  src_sub2->AddField( Fnv32( "var2" ), kField_F32 );
  src_sub2->FixSizeAndStride();

  Aggregate src_agg( 4 );
  src_agg.AddField( Fnv32( "var1" ), kField_F32 );
  src_agg.AddField( Fnv32( "var2" ), kField_F32 );
  src_agg.AddSubStructTemp( Fnv32( "sub1" ), src_sub1 );
  src_agg.AddSubStructTemp( Fnv32( "sub2" ), src_sub2 );
  src_agg.FixSizeAndStride();

  Aggregate* dst_sub1 = new Aggregate( 2 );
  dst_sub1->AddField( Fnv32( "var2" ), kField_F32 );
  dst_sub1->AddField( Fnv32( "var1" ), kField_F32 );
  dst_sub1->FixSizeAndStride();

  Aggregate* dst_sub2 = new Aggregate( 2 );
  dst_sub2->AddField( Fnv32( "var1" ), kField_F32 );
  dst_sub2->AddField( Fnv32( "var2" ), kField_F32 );
  dst_sub2->FixSizeAndStride();

  Aggregate dst_agg( 4 );
  dst_agg.AddField    ( Fnv32( "var2" ), kField_F32 );
  dst_agg.AddSubStructTemp( Fnv32( "sub1" ), dst_sub1 );
  dst_agg.AddSubStructTemp( Fnv32( "sub2" ), dst_sub2 );
  dst_agg.AddField    ( Fnv32( "var1" ), kField_F32 );
  dst_agg.FixSizeAndStride();
  
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
  
  dst_agg.Convert( dst_data, ReadCursor( src_data, 0, &src_agg ) );

  if( dst.var1      != src.var1 )       return "var1 is wrong";
  if( dst.var2      != src.var2 )       return "var2 is wrong";
  if( dst.sub1.var1 != src.sub1.var1 )  return "sub1.var1 is wrong";
  if( dst.sub1.var2 != src.sub1.var2 )  return "sub1.var2 is wrong";
  if( dst.sub2.var1 != src.sub2.var1 )  return "sub2.var1 is wrong";
  if( dst.sub2.var2 != src.sub2.var2 )  return "sub2.var2 is wrong";

  return NULL;
}
