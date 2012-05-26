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
#include <stdint.h>

class sbByteWriter
{
public:
  sbByteWriter( char* start, char* end );
  void Write8( uint8_t value );
  void Write16( uint16_t value );
  void Write32( uint32_t value );
  void Write64( uint64_t value );
  int GetSize() const;
  
private:
  char* m_Start;
  char* m_End;
  char* m_Pointer;
};

#endif
