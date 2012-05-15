//
//  Fnv.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "Fnv.h"
// Libraries
#include <stdint.h>
// Project

#define kFnvPrimeU32 16777619
#define kFnvBasisU32 2166136261
#define kFnvPrimeU64 1099511628211ULL
#define kFnvBasisU64 14695981039346656037ULL

uint32_t Fnv32( const char* s )
{
  uint32_t hash = kFnvBasisU32;
  for( char c = *s++; c; c = *s++ )
  {
    hash = ( hash ^ c ) * kFnvPrimeU32;
  }
  return hash;
}

uint32_t Fnv32( const char* s, int count )
{
  uint32_t hash = kFnvBasisU32;
  for( int i = 0; i < count; ++i )
  {
    hash = ( hash ^ s[ i ] ) * kFnvPrimeU32;
  }
  return hash;
}

uint64_t Fnv64( const char* s )
{
  uint64_t hash = kFnvBasisU64;
  for( char c = *s++; c; c = *s++ )
  {
    hash = ( hash ^ c ) * kFnvPrimeU64;
  }
  return hash;
}

uint64_t Fnv64( const char* s, int count )
{
  uint64_t hash = kFnvBasisU64;
  for( int i = 0; i < count; ++i )
  {
    hash = ( hash ^ s[ i ] ) * kFnvPrimeU64;
  }
  return hash;
}

