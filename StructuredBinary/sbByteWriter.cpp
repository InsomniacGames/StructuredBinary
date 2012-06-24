//
//  sbByteWriter.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/24/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "sbByteWriter.h"

// Libraries
#include <stdint.h>
#include <assert.h>

sbByteWriter::sbByteWriter( char* data, size_t size )
{
  SetBuffer( data, size );
}

sbByteWriter::sbByteWriter()
{
  SetBuffer( NULL, 0 );
}

void sbByteWriter::SetBuffer( char* data, size_t size )
{
  m_Data = data;
  m_Size = size;
  m_Offset = 0;
}

char* sbByteWriter::GetBuffer() const
{
  return m_Data;
}

void sbByteWriter::Seek( size_t pos )
{
  m_Offset = pos;
}

size_t sbByteWriter::Tell() const
{
  return m_Offset;
}

void sbByteWriter::Write8( uint8_t value )
{
  if( m_Data )
  {
    assert( m_Offset < m_Size );
    m_Data[ m_Offset ] = value;
  }
  m_Offset += 1;
}

void sbByteWriter::Write16( uint16_t value )
{
  Write8( ( uint8_t )( value >> 8 ) );
  Write8( ( uint8_t )( value >> 0 ) );
}

void sbByteWriter::Write32( uint32_t value )
{
  Write16( ( uint16_t )( value >> 16 ) );
  Write16( ( uint16_t )( value >>  0 ) );
}

void sbByteWriter::Write64( uint64_t value )
{
  Write32( ( uint32_t )( value >> 32 ) );
  Write32( ( uint32_t )( value >>  0 ) );
}
