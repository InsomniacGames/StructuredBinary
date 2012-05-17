//
//  UnitTestChunkFile.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "UnitTestChunkFile.h"

// Libraries
#include <cstring>
#include <stdio.h>
#include <stdint.h>

// Project
#include "Chunk.h"
#include "ChunkFile.h"

const char* UnitTestChunkFile::RunTest() const
{
  const char* sweet = "sweet data";
  const char* salt = "salt data";
  const char* bitter = "bitter data";
  const char* sour = "sour data";
  const char* umami = "umami data";
  
  uint32_t sweet_size   = ( uint32_t )strlen( sweet   ) + 1;
  uint32_t salt_size    = ( uint32_t )strlen( salt    ) + 1;
  uint32_t bitter_size  = ( uint32_t )strlen( bitter  ) + 1;
  uint32_t sour_size    = ( uint32_t )strlen( sour    ) + 1;
  uint32_t umami_size   = ( uint32_t )strlen( umami   ) + 1;
  
  Chunk chunk( "top" );
  chunk.AddLeafChunk( "sweet", sweet, sweet_size );
  chunk.AddLeafChunk( "salty", salt, salt_size );
  
  Chunk* child = chunk.AddChunk( "child" );
  child->AddLeafChunk( "sour", sour, sour_size );
  child->AddLeafChunk( "bitter", bitter, bitter_size );
  child->AddLeafChunk( "umami", umami, umami_size );

  bool success = ChunkWrite( "RonsTestFile.chunk", &chunk );
  if( !success ) return "Could not write file";
  
  return NULL;
}
