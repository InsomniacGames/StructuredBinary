//
//  sbHash.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbFnv_h
#define StructuredBinary_sbFnv_h

#include "stdint.h"

uint32_t sbFnv32( const char* s );
uint32_t sbFnv32( const char* s, int count );
uint64_t sbFnv64( const char* s );
uint64_t sbFnv64( const char* s, int count );

typedef const char* ( sbHashLookupCallback32 )( uint32_t, char* );
typedef const char* ( sbHashLookupCallback64 )( uint64_t, char* );

class sbHash
{
public:
  sbHash( const char* text );
  sbHash( uint32_t value );
  sbHash();
  operator const uint32_t() const;
  bool operator== ( const sbHash& other ) const;

private:
  uint32_t  m_Value;
};

#endif
