//
//  UnitTestFormatFile2.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/27/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "TestFormatFile2.h"

// Libraries
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

// Project
#include "sbHash.h"
#include "sbChunk.h"
#include "sbChunkFile.h"
#include "sbByteReader.h"
#include "sbByteWriter.h"

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

UnitTest::Result TestFormatFile2::RunTest() const
{
/*
  sbStruct* src_sub1 = new sbStruct;
  src_sub1->AddScalar( sbFnv32( "var1" ), sbTypeType_F32 );
  src_sub1->AddScalar( sbFnv32( "var2" ), sbTypeType_F32 );
  
  sbStruct* src_sub2 = new sbStruct;
  src_sub2->AddScalar( sbFnv32( "var1" ), sbTypeType_F32 );
  src_sub2->AddScalar( sbFnv32( "var2" ), sbTypeType_F32 );
  
  sbStruct org_agg;
  org_agg.AddScalar( sbFnv32( "var1" ), sbTypeType_F32 );
  org_agg.AddScalar( sbFnv32( "var2" ), sbTypeType_F32 );
  org_agg.AddStruct( sbFnv32( "sub1" ), src_sub1 );
  org_agg.AddStruct( sbFnv32( "sub2" ), src_sub2 );

  char format_buffer[ 1000 ];
  sbByteWriter w( format_buffer, format_buffer + sizeof( format_buffer ) );
  org_agg.WriteSchema( &w );
  
  int write_size = w.GetSize();
//  printf( "size = %d\n", write_size );
  sbByteReader r( format_buffer, format_buffer + write_size );
  const sbStruct* src_agg = ( const sbStruct* )sbStruct::NewFromSchema( &r );

  sbStruct* dst_sub1 = new sbStruct;
  dst_sub1->AddScalar( sbFnv32( "var2" ), sbTypeType_F32 );
  dst_sub1->AddScalar( sbFnv32( "var1" ), sbTypeType_F32 );
  
  sbStruct* dst_sub2 = new sbStruct;
  dst_sub2->AddScalar( sbFnv32( "var1" ), sbTypeType_F32 );
  dst_sub2->AddScalar( sbFnv32( "var2" ), sbTypeType_F32 );
  
  sbStruct dst_agg;
  dst_agg.AddScalar    ( sbFnv32( "var2" ), sbTypeType_F32 );
  dst_agg.AddStruct( sbFnv32( "sub1" ), dst_sub1 );
  dst_agg.AddStruct( sbFnv32( "sub2" ), dst_sub2 );
  dst_agg.AddScalar    ( sbFnv32( "var1" ), sbTypeType_F32 );
  
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
  
  dst_agg.Convert( dst_data, src_data, src_agg );
  
  if( dst.var1      != src.var1 )       return "var1 is wrong";
  if( dst.var2      != src.var2 )       return "var2 is wrong";
  if( dst.sub1.var1 != src.sub1.var1 )  return "sub1.var1 is wrong";
  if( dst.sub1.var2 != src.sub1.var2 )  return "sub1.var2 is wrong";
  if( dst.sub2.var1 != src.sub2.var1 )  return "sub2.var1 is wrong";
  if( dst.sub2.var2 != src.sub2.var2 )  return "sub2.var2 is wrong";

  delete src_agg;
*/
  return Ok();
}
