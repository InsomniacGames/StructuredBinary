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

uint32_t GetCompleteSize( const Chunk* chunk )
{  
  uint32_t size = 8;

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
      size += GetCompleteSize( child );
      child = child->GetSibling();
    }
  }
  return size;
}

void ChunkWrite( FILE* file, const Chunk* chunk )
{
  uint32_t name = Fnv32( chunk->GetName() );
  uint32_t size = GetCompleteSize( chunk );
  
  fwrite( &name, 1, sizeof( name ), file );
  fwrite( &size, 1, sizeof( size ), file );

  const void* data = chunk->GetData();
  if( data )
  {
    uint32_t data_size = chunk->GetDataSize();
    fwrite( data, 1, data_size, file );
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
