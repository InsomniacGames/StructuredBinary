//
//  ChunkFile.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "ChunkFile.h"
#include "Fnv.h"

#include <ctype.h>
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

int ChunkWrite( const char* file_name, const Chunk* chunk )
{
  FILE* file = fopen( file_name, "wb" );
  if( !file ) return 0;
  ChunkWrite( file, chunk );
  fseek( file, 0, SEEK_END );
  int size = ( int )ftell( file );
  fclose( file );
  return size;
}

static int GrabChunk( Chunk* parent_chunk, const char* data_start, const char* data_end )
{
  int grabbed = 0;
  uint32_t chunk_id = *( const uint32_t* )data_start;
  uint32_t chunk_size = *( const uint32_t* )( data_start + 4 );

  char first_char = data_start[ 0 ];
  if( isupper( first_char ) )
  {
    // This is a container chunk
    Chunk* chunk = parent_chunk->AddChunk( chunk_id );
    const char* chunk_start = data_start + 8;
    const char* chunk_end = chunk_start + chunk_size;
    
    while( chunk_start < chunk_end )
    {
      int gr = GrabChunk( chunk, chunk_start, chunk_end );
      // ASSERT if gr == 0
      chunk_start += gr;
      chunk_start += ( -( intptr_t )chunk_start ) & 3;
    }
    grabbed = ( int )( chunk_start - data_start );
  }
  else
  {
    // This is a leaf chunk
    parent_chunk->AddLeafChunk( chunk_id, data_start + 8, chunk_size ); // 8 = skip header 
    grabbed = 8 + chunk_size; // 8 = size of header
  }

  return grabbed;
}

const Chunk* ChunkRead( const char* file_name, char* buffer, int buffer_size )
{
  Chunk* chunk = NULL;
  FILE* file = fopen( file_name, "rb" );
  if( file )
  {
    fseek( file, 0, SEEK_END );
    int file_size = ( int )ftell( file );
    fseek( file, 0, SEEK_SET );
    if( file_size <= buffer_size )
    {
      fread( buffer, 1, file_size, file );
      chunk = new Chunk( 0 );
      GrabChunk( chunk, buffer, buffer + file_size );
    }
    fclose( file );
  }
  return chunk;
}

