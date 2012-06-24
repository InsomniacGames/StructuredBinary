//
//  sbByteReader.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/24/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbByteReader_h
#define StructuredBinary_sbByteReader_h

// Libraries
#include <stddef.h>
#include <stdint.h>

class sbByteReader
{
public:
  sbByteReader();
  sbByteReader( const char* data, size_t size );
  void SetBuffer( const char* data, size_t size );
  const char* GetBuffer() const;
  uint8_t Read8();
  uint16_t Read16();
  uint32_t Read32();
  uint64_t Read64();
  size_t GetRemain() const;

  void Seek( size_t pos = 0 );
  size_t Tell() const;

  template< typename T >
  T* ReadNew()
  {
    return T::ReadNew( this );
  }
  
  template< typename T >
  T Read()
  {
    return T::Read( this );
  }
  
private:
  const char* m_Data;
  size_t      m_Size;
  size_t      m_Offset;
};

#endif
