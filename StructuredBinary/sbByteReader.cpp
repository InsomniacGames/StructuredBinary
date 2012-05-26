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

sbByteReader::sbByteReader( const char* start, const char* end )
: m_Start( start )
, m_End( end )
, m_Pointer( start )
{}

void sbByteReader::Rewind()
{
  m_Pointer = m_Start;
}

uint8_t sbByteReader::Read8()
{
  assert( m_Pointer < m_End );
  return *m_Pointer++;
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

int sbByteReader::GetRemain() const
{
  return ( int )( m_End - m_Pointer );
}
