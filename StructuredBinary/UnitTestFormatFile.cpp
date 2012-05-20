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

// Project
#include "Fnv.h"
#include "Aggregate.h"
#include "Chunk.h"
#include "ChunkFile.h"

enum FieldType
{
  kField_Unknown = 0,
  
  kField_I8,
  kField_U8,
  
  kField_I16,
  kField_U16,
  
  kField_I32,
  kField_U32,
  
  kField_I64,
  kField_U64,
  
  kField_F32,
  kField_F64,
  
  kField_Count
};

class Writer
{
public:
  Writer( char* start, char* end )
  : m_Start( start )
  , m_End( end )
  , m_Pointer( start )
  {}
  
  void Write8( uint8_t value )
  {
    *m_Pointer++ = value;
  }
  
  void Write16( uint16_t value )
  {
    Write8( ( uint8_t )( value >> 8 ) );
    Write8( ( uint8_t )( value >> 0 ) );
  }
  
  void Write32( uint32_t value )
  {
    Write16( ( uint16_t )( value >> 16 ) );
    Write16( ( uint16_t )( value >>  0 ) );
  }
  
  void Write64( uint64_t value )
  {
    Write32( ( uint32_t )( value >> 32 ) );
    Write32( ( uint32_t )( value >>  0 ) );
  }
  
  int GetSize() const
  {
    return ( int )( m_Pointer - m_Start );
  }
  
private:
  char* m_Start;
  char* m_End;
  char* m_Pointer;
};

class Reader
{
public:
  Reader( const char* start, const char* end )
  : m_Start( start )
  , m_End( end )
  , m_Pointer( start )
  {}
  
  uint8_t Read8()
  {
    return *m_Pointer++;
  }
  
  uint16_t Read16()
  {
    uint16_t hi = Read8();
    uint16_t lo = Read8();
    return ( hi << 8 ) | lo;
  }
  
  uint32_t Read32()
  {
    uint32_t hi = Read16();
    uint32_t lo = Read16();
    return ( hi << 16 ) | lo;
  }
  
  uint64_t Read64()
  {
    uint64_t hi = Read32();
    uint64_t lo = Read32();
    return ( hi << 32 ) | lo;
  }

  int GetRemain() const
  {
    return ( int )( m_End - m_Pointer );
  }
  
private:
  const char* m_Start;
  const char* m_End;
  const char* m_Pointer;
};

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
  char format_buffer[ 1000 ];
  Writer w( format_buffer, format_buffer + sizeof( format_buffer ) );
  
  w.Write32( Fnv32( "f64" ) );
  w.Write8( kField_F64 );

  w.Write32( Fnv32( "i64" ) );
  w.Write8( kField_I64 );

  w.Write32( Fnv32( "u64" ) );
  w.Write8( kField_U64 );

  w.Write32( Fnv32( "f32" ) );
  w.Write8( kField_F32 );

  w.Write32( Fnv32( "i32" ) );
  w.Write8( kField_I32 );
  
  w.Write32( Fnv32( "u32" ) );
  w.Write8( kField_U32 );
  
  w.Write32( Fnv32( "i16" ) );
  w.Write8( kField_I16 );

  w.Write32( Fnv32( "u16" ) );
  w.Write8( kField_U16 );

  w.Write32( Fnv32( "i8" ) );
  w.Write8( kField_I8 );

  w.Write32( Fnv32( "u8" ) );
  w.Write8( kField_U8 );
  int write_size = w.GetSize();
  
  Reader r( format_buffer, format_buffer + write_size );
  Aggregate src_agg( 10 );
  while( r.GetRemain() > 0 )
  {
    uint32_t name = r.Read32();
    uint8_t field_type = r.Read8();

    switch( field_type )
    {
      case kField_I8:
        src_agg.AddInt8( name );
        break;
      case kField_U8:
        src_agg.AddUInt8( name );
        break;
      case kField_I16:
        src_agg.AddInt16( name );
        break;
      case kField_U16:
        src_agg.AddUInt16( name );
        break;
      case kField_I32:
        src_agg.AddInt32( name );
        break;
      case kField_U32:
        src_agg.AddUInt32( name );
        break;
      case kField_I64:
        src_agg.AddInt64( name );
        break;
      case kField_U64:
        src_agg.AddUInt64( name );
        break;
      case kField_F32:
        src_agg.AddFloat32( name );
        break;
      case kField_F64:
        src_agg.AddFloat64( name );
        break;
    }
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
