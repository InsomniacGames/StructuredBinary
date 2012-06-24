//
//  sbByteReader.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/24/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "sbByteReader.h"

// Libraries
#include <stdint.h>
#include <assert.h>

sbByteReader::sbByteReader( const char* data, size_t size )
{
  SetBuffer( data, size );
}

sbByteReader::sbByteReader()
{
  SetBuffer( NULL, 0 );
}

void sbByteReader::SetBuffer( const char* data, size_t size )
{
  m_Data = data;
  m_Size = size;
  m_Offset = 0;
}

const char* sbByteReader::GetBuffer() const
{
  return m_Data;
}

void sbByteReader::Seek( size_t pos )
{
  m_Offset = pos;
}

size_t sbByteReader::Tell() const
{
  return m_Offset;
}

uint8_t sbByteReader::Read8()
{
  assert( m_Offset < m_Size );
  return m_Data[ m_Offset++ ];
}

uint16_t sbByteReader::Read16()
{
  uint16_t hi = Read8();
  uint16_t lo = Read8();
  return ( hi << 8 ) | lo;
}

uint32_t sbByteReader::Read32()
{
  uint32_t hi = Read16();
  uint32_t lo = Read16();
  return ( hi << 16 ) | lo;
}

uint64_t sbByteReader::Read64()
{
  uint64_t hi = Read32();
  uint64_t lo = Read32();
  return ( hi << 32 ) | lo;
}

size_t sbByteReader::GetRemain() const
{
  return m_Size - m_Offset;
}
