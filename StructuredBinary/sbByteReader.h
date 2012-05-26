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
#include <stdint.h>

class sbByteReader
{
public:
  sbByteReader( const char* start, const char* end );
  uint8_t Read8();
  uint16_t Read16();
  uint32_t Read32();
  uint64_t Read64();
  int GetRemain() const;
  void Rewind();

private:
  const char* m_Start;
  const char* m_End;
  const char* m_Pointer;
};

#endif
