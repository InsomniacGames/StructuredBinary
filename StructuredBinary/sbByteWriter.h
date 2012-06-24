//
//  sbByteWriter.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/24/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbByteWriter_h
#define StructuredBinary_sbByteWriter_h

// Libraries
#include <stddef.h>
#include <stdint.h>

class sbByteWriter
{
public:
  sbByteWriter();
  sbByteWriter( char* data, size_t size );
  void SetBuffer( char* data, size_t size );
  char* GetBuffer() const;
  void Write8( uint8_t value );
  void Write16( uint16_t value );
  void Write32( uint32_t value );
  void Write64( uint64_t value );

  void Seek( size_t pos = 0 );
  size_t Tell() const;

  template< typename T >
  void Write( const T t )
  {
    t->Write( this );
  }

private:
  char*   m_Data;
  size_t  m_Size;
  size_t  m_Offset;
};

#endif
