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
  const uint32_t top_id    = CHUNK_ID( 'S', 'T', 'R', 'B' );
  const uint32_t child_id  = CHUNK_ID( 'C', 'H', 'L', 'D' );
  const uint32_t sweet_id  = CHUNK_ID( 's', 'w', 'e', 'e' );
  const uint32_t salt_id   = CHUNK_ID( 's', 'a', 'l', 't' );
  const uint32_t bitter_id = CHUNK_ID( 'b', 't', 't', 'r' );
  const uint32_t sour_id   = CHUNK_ID( 's', 'o', 'u', 'r' );
  const uint32_t umami_id  = CHUNK_ID( 'u', 'm', 'a', 'm' );

  const char* sweet_data  = "Sugar&Honey";
  const char* salt_data   = "Chips&Jerky";
  const char* bitter_data = "Chocolate&Grapefruit";
  const char* sour_data   = "Lemon&Vinegar";
  const char* umami_data  = "Asparagus&Cheese";
  
  uint32_t sweet_size   = ( uint32_t )strlen( sweet_data  ) + 1;
  uint32_t salt_size    = ( uint32_t )strlen( salt_data   ) + 1;
  uint32_t bitter_size  = ( uint32_t )strlen( bitter_data ) + 1;
  uint32_t sour_size    = ( uint32_t )strlen( sour_data   ) + 1;
  uint32_t umami_size   = ( uint32_t )strlen( umami_data  ) + 1;
  
  Chunk chunk( top_id );
  chunk.AddLeafChunk( sweet_id, sweet_data, sweet_size );
  chunk.AddLeafChunk( salt_id, salt_data, salt_size );
  
  Chunk* child = chunk.AddChunk( child_id );
  child->AddLeafChunk( sour_id, sour_data, sour_size );
  child->AddLeafChunk( bitter_id, bitter_data, bitter_size );
  child->AddLeafChunk( umami_id, umami_data, umami_size );

  bool success = ChunkWrite( "RonsTestFile.chunk", &chunk );
  if( !success ) return "Could not write file";
  
  return NULL;
}
