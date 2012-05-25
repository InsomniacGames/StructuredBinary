//
//  ByteReader.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/24/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "ByteReader.h"

// Libraries
#include <stdint.h>
#include <assert.h>

ByteReader::ByteReader( const char* start, const char* end )
: m_Start( start )
, m_End( end )
, m_Pointer( start )
{}

uint8_t ByteReader::Read8()
{
  assert( m_Pointer < m_End );
  return *m_Pointer++;
}

uint16_t ByteReader::Read16()
{
  uint16_t hi = Read8();
  uint16_t lo = Read8();
  return ( hi << 8 ) | lo;
}

uint32_t ByteReader::Read32()
{
  uint32_t hi = Read16();
  uint32_t lo = Read16();
  return ( hi << 16 ) | lo;
}

uint64_t ByteReader::Read64()
{
  uint64_t hi = Read32();
  uint64_t lo = Read32();
  return ( hi << 32 ) | lo;
}

int ByteReader::GetRemain() const
{
  return ( int )( m_End - m_Pointer );
}
