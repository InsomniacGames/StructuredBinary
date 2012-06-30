//
//  TestConvertFileFlat.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/30/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "TestConvertFileFlat.h"

#include <stdio.h>

#include "sbSchema.h"
#include "sbAggregateType.h"
#include "sbByteWriter.h"
#include "sbByteReader.h"
#include "sbChunkFile.h"

static char FileBuffer[ 1000 ];
static const char* s_FileName = "TestConvertFileFlat.chunk";
static const uint32_t top_id     = CHUNK_ID( 'S', 'B', '4', 'G' );
static const uint32_t schema_id  = CHUNK_ID( 's', 'c', 'm', 'a' );
static const uint32_t pay_id     = CHUNK_ID( 'p', 'a', 'y', ' ' );

namespace Src
{
  // Alignment jumbled up on purpose!
  struct Struct
  {
    uint8_t   u8;
    float     f32;
    uint16_t  u16;
    uint32_t  u32;
    uint64_t  u64;
    int8_t    i8;
    double    f64;
    int16_t   i16;
    int32_t   i32;
    int64_t   i64;
  };
};

UnitTest::Result TestConvertFileFlat::TestWrite() const
{
  sbSchema src_schema;
  src_schema.Begin();
  
  src_schema.BeginAggregate( "Struct" );
  src_schema.AddInstance( "u8" , 1, "uint8_t");
  src_schema.AddInstance( "f32", 1, "float" );
  src_schema.AddInstance( "u16", 1, "uint16_t" );
  src_schema.AddInstance( "u32", 1, "uint32_t" );
  src_schema.AddInstance( "u64", 1, "uint64_t" );
  src_schema.AddInstance( "i8" , 1, "int8_t"  );
  src_schema.AddInstance( "f64", 1, "double" );
  src_schema.AddInstance( "i16", 1, "int16_t" );
  src_schema.AddInstance( "i32", 1, "int32_t" );
  src_schema.AddInstance( "i64", 1, "int64_t" );
  src_schema.EndAggregate();
  
  src_schema.End();

  Src::Struct src_struct;
  src_struct.u8   = 200;
  src_struct.f32  = 3.141f;
  src_struct.u16  = 60000;
  src_struct.u32  = 12345;
  src_struct.u64  = 0xde53a0914a8420efULL;
  src_struct.i8   = -100;
  src_struct.f64  = 1.23456789;
  src_struct.i16  = -30;
  src_struct.i32  = 0x23eda091;
  src_struct.i64  = 0x193d59739a0ca2c1LL;

  char src_data_buffer[ 1000 ];
  sbAllocator alloc = sbAllocator( src_data_buffer, sizeof( src_data_buffer ) );
  
  char* src_data = src_schema.Convert( ( const char* )&src_struct, &src_schema, "Struct", &alloc );
  size_t src_data_size = alloc.GetSize();
  
  printf( "Source data is %lu bytes\n", src_data_size );

  sbByteWriter writer;
  writer.Write( &src_schema );
  
  size_t calculated_size = writer.Tell();
  
  char* src_schema_data = new char[ calculated_size ];
  writer.SetBuffer( src_schema_data, calculated_size );
  
  writer.Write( &src_schema );
  size_t src_schema_size = writer.Tell();
  
  printf( "Source schema is %lu bytes\n", src_schema_size );
  
  
  sbChunk* top_chunk = new sbChunk( top_id );
  top_chunk->AddChild( new sbChunk( pay_id,    src_data, ( int )src_data_size ) );
  top_chunk->AddChild( new sbChunk( schema_id, src_schema_data, ( int )src_schema_size ) );
  
  int file_size = sbChunkWrite( s_FileName, top_chunk );
  if( file_size == 0 ) return Error( "Could not write file" );
  
  printf( "Written %lu bytes\n", ( size_t )file_size );

  delete top_chunk;
  delete src_schema_data;
  return Ok();
}

namespace Dst
{
  // Alignment jumbled up on purpose!
  struct Struct
  {
    uint64_t  u64;
    int64_t   i64;
    double    f64;
    int32_t   i32;
    uint32_t  u32;
    float     f32;
    uint16_t  u16;
    int16_t   i16;
    int8_t    i8;
    uint8_t   u8;
  };
};

UnitTest::Result TestConvertFileFlat::TestRead() const
{
  sbSchema dst_schema;
  dst_schema.Begin();
  
  dst_schema.BeginAggregate( "Struct" );
  dst_schema.AddInstance( "u64", 1, "uint64_t" );
  dst_schema.AddInstance( "i64", 1, "int64_t" );
  dst_schema.AddInstance( "f64", 1, "double" );
  dst_schema.AddInstance( "i32", 1, "int32_t" );
  dst_schema.AddInstance( "u32", 1, "uint32_t" );
  dst_schema.AddInstance( "f32", 1, "float" );
  dst_schema.AddInstance( "u16", 1, "uint16_t" );
  dst_schema.AddInstance( "i16", 1, "int16_t" );
  dst_schema.AddInstance( "i8" , 1, "int8_t" );
  dst_schema.AddInstance( "u8" , 1, "uint8_t" );
  dst_schema.EndAggregate();
  
  dst_schema.End();

  const sbChunk* read_root = sbChunkRead( s_FileName, FileBuffer, sizeof( FileBuffer ) );
  if( !read_root ) return Error( "Failed to read file" );
  
  const sbChunk* pay_chunk = read_root->FindChild( pay_id );
  if( !pay_chunk ) return Error( "Could not locate payload chunk" );
  const sbChunk* schema_chunk = read_root->FindChild( schema_id );
  if( !schema_chunk ) return Error( "Could not locate schema chunk" );

//  sbByteReader reader( schema_chunk->GetData(), ( uint32_t )schema_chunk->GetDataSize() );

  const char* src_data = pay_chunk->GetData();
  uint32_t src_data_size = ( uint32_t )pay_chunk->GetDataSize();

  const char* schema_data = schema_chunk->GetData();
  uint32_t schema_data_size = ( uint32_t )schema_chunk->GetDataSize();
  sbByteReader reader( schema_data, schema_data_size );

  sbSchema* src_schema = reader.ReadNew< sbSchema >();
  if( !src_schema ) return Error( "schema could not be read" );


  sbAllocator alloc;
  dst_schema.Convert( src_data, src_schema, "Struct", &alloc );
  size_t dst_data_size = alloc.GetSize();
  
  printf( "Size after conversion will be %lu bytes\n", dst_data_size );

  char* dst_buffer = new char[ dst_data_size ];

  alloc.SetBuffer( dst_buffer, dst_data_size );

  Dst::Struct* dst_struct = ( Dst::Struct* )dst_schema.Convert( src_data, src_schema, "Struct", &alloc );
  
  if( dst_struct->u8   != 200 )                  return Error( "u8 wrong value" );
  if( dst_struct->f32  != 3.141f )               return Error( "f32 wrong value" );
  if( dst_struct->u16  != 60000 )                return Error( "u16 wrong value" );
  if( dst_struct->u32  != 12345 )                return Error( "u32 wrong value" );
  if( dst_struct->u64  != 0xde53a0914a8420efLL ) return Error( "u64 wrong value" );
  if( dst_struct->i8   != -100 )                 return Error( "i8 wrong value" );
  if( dst_struct->f64  != 1.23456789 )           return Error( "f64 wrong value" );
  if( dst_struct->i16  != -30 )                  return Error( "i16 wrong value" );
  if( dst_struct->i32  != 0x23eda091 )           return Error( "i32 wrong value" );
  if( dst_struct->i64  != 0x193d59739a0ca2c1LL ) return Error( "164 wrong value" );

  delete dst_buffer;
  delete src_schema;
  delete read_root;
  return Ok();
}

UnitTest::Result TestConvertFileFlat::RunTest() const
{
  Result result = TestWrite();
  if( !result.IsSuccess() ) return result;
  
  result = TestRead();
  if( !result.IsSuccess() ) return result;

  return Ok();
}
