//
//  ChunkFile.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "ChunkFile.h"
#include "Fnv.h"

#include <stdio.h>

static const int s_HeaderSize = 8;

static uint32_t GetDataSize( const Chunk* chunk )
{  
  uint32_t size = 0;

  const void* data = chunk->GetData();
  if( data )
  {
    size += chunk->GetDataSize();
  }
  else
  {
    const Chunk* child = chunk->GetChild();
    while( child )
    {
      size += GetDataSize( child );
      size += 8;              // Add size of header
      size += ( -size & 3 );  // Round up to nearest 4 bytes
      child = child->GetSibling();
    }
  }
  return size;
}

static void ChunkWrite( FILE* file, const Chunk* chunk )
{
  uint32_t id = chunk->GetId();
  uint32_t data_size = GetDataSize( chunk );
  uint32_t pad = 0xcccccccc;
  
  fwrite( &id, 1, sizeof( id ), file );
  fwrite( &data_size, 1, sizeof( data_size ), file );

  const void* data = chunk->GetData();
  if( data )
  {
    uint32_t data_size = chunk->GetDataSize();
    fwrite( data, 1, data_size, file );
    
    int pad_size = ( -data_size & 3 );
    fwrite( &pad, 1, pad_size, file );
  }
  else
  {
    const Chunk* child = chunk->GetChild();
    while( child )
    {
      ChunkWrite( file, child );
      child = child->GetSibling();
    }
  }
}

bool ChunkWrite( const char* file_name, const Chunk* chunk )
{
  FILE* file = fopen( file_name, "wb" );
  if( !file ) return false;
  ChunkWrite( file, chunk );
  return true;
}
