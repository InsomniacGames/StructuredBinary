//
//  UnitTestFormatFile.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/19/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "UnitTestFormatFile.h"

// Libraries
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

// Project
#include "Fnv.h"
#include "Aggregate.h"
#include "Chunk.h"
#include "ChunkFile.h"
#include "ByteReader.h"
#include "ByteWriter.h"

struct ReadStruct
{
  double    f64;
  int64_t   i64;
  uint64_t  u64;
  float     f32;
  int32_t   i32;
  uint32_t  u32;
  int16_t   i16;
  uint16_t  u16;
  int8_t    i8;
  uint8_t   u8;
};


const char* UnitTestFormatFile::RunTest() const
{
  Aggregate org_agg( 10 );
  org_agg.AddField( Fnv32( "f64" ), kField_F64 );
  org_agg.AddField( Fnv32( "i64" ), kField_I64 );
  org_agg.AddField( Fnv32( "u64" ), kField_U64 );
  org_agg.AddField( Fnv32( "f32" ), kField_F32 );
  org_agg.AddField( Fnv32( "i32" ), kField_I32 );
  org_agg.AddField( Fnv32( "u32" ), kField_U32 );
  org_agg.AddField( Fnv32( "i16" ), kField_I16 );
  org_agg.AddField( Fnv32( "u16" ), kField_U16 );
  org_agg.AddField( Fnv32( "i8"  ), kField_I8  );
  org_agg.AddField( Fnv32( "u8"  ), kField_U8  );
  org_agg.FixSizeAndStride();

  char format_buffer[ 1000 ];
  ByteWriter w( format_buffer, format_buffer + sizeof( format_buffer ) );

  org_agg.WriteFormat( &w );
  
  int write_size = w.GetSize();

  ByteReader r( format_buffer, format_buffer + write_size );
  Aggregate src_agg( 10 );
  while( r.GetRemain() > 0 )
  {
    uint32_t name = r.Read32();
    uint8_t field_type = r.Read8();
    src_agg.AddField( name, ( FieldType )field_type );
  }
  src_agg.FixSizeAndStride();

  ReadStruct s;
  s.f64 = 3.14159265358979;
  s.i64 = 0x69b0aee3db807b41ULL;
  s.u64 = 0x0dbf9bf837f5adb0ULL;
  s.f32 = 2.71828f;
  s.i32 = 0xc4aa8c4a;
  s.u32 = 0x140bbf84;
  s.i16 = 0x4a90;
  s.u16 = 0x6eb6;
  s.i8  = 0xc3;
  s.u8  = 0x40;
  const char* data = ( const char* )&s;
  
  Number n;
  n = src_agg.Read( data, Fnv32( "f64" ) );
  if( !n.IsFloat() )          return "f64 returned wrong type";
  if( n.AsFloat() != s.f64 )  return "f64 returned wrong value";
  
  n = src_agg.Read( data, Fnv32( "i64" ) );
  if( !n.IsInt() )            return "i64 returned wrong type";
  if( n.AsFloat() != s.i64 )  return "i64 returned wrong value";
  
  n = src_agg.Read( data, Fnv32( "u64" ) );
  if( !n.IsInt() )            return "u64 returned wrong type";
  if( n.AsFloat() != s.u64 )  return "u64 returned wrong value";
  
  n = src_agg.Read( data, Fnv32( "f32" ) );
  if( !n.IsFloat() )          return "f32 returned wrong type";
  if( n.AsFloat() != s.f32 )  return "f32 returned wrong value";
  
  n = src_agg.Read( data, Fnv32( "i32" ) );
  if( !n.IsInt() )            return "i32 returned wrong type";
  if( n.AsFloat() != s.i32 )  return "i32 returned wrong value";
  
  n = src_agg.Read( data, Fnv32( "u32" ) );
  if( !n.IsInt() )            return "u32 returned wrong type";
  if( n.AsFloat() != s.u32 )  return "u32 returned wrong value";
  
  n = src_agg.Read( data, Fnv32( "i16" ) );
  if( !n.IsInt() )            return "i16 returned wrong type";
  if( n.AsFloat() != s.i16 )  return "i16 returned wrong value";
  
  n = src_agg.Read( data, Fnv32( "u8" ) );
  if( !n.IsInt() )            return "u8 returned wrong type";
  if( n.AsFloat() != s.u8 )   return "u8 returned wrong value";
  
  return NULL;
}
