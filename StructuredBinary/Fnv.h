//
//  Fnv.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_Fnv_h
#define StructuredBinary_Fnv_h

#include "stdint.h"

uint32_t Fnv32( const char* s );
uint32_t Fnv32( const char* s, int count );
uint64_t Fnv64( const char* s );
uint64_t Fnv64( const char* s, int count );

typedef const char* ( HashLookupCallback32 )( uint32_t, char* );
typedef const char* ( HashLookupCallback64 )( uint64_t, char* );

#endif
