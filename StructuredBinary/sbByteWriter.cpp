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

sbByteWriter::sbByteWriter( char* start, char* end )
: m_Start( start )
, m_End( end )
, m_Pointer( start )
{}

void sbByteWriter::Write8( uint8_t value )
{
  assert( m_Pointer < m_End );
  *m_Pointer++ = value;
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

int sbByteWriter::GetSize() const
{
  return ( int )( m_Pointer - m_Start );
}
