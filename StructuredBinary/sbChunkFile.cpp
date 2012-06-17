//
//  sbChunkFile.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbChunkFile.h"
#include "sbHash.h"

#include <ctype.h>
#include <stdio.h>

static const int s_HeaderSize = 8;

static uint32_t GetDataSize( const sbChunk* chunk )
{  
  uint32_t size = 0;

  const void* data = chunk->GetData();
  if( data )
  {
    size += chunk->GetDataSize();
  }
  else
  {
    const sbChunk* child = chunk->GetChild();
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

static void ChunkWrite( FILE* file, const sbChunk* chunk )
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
    const sbChunk* child = chunk->GetChild();
    while( child )
    {
      ChunkWrite( file, child );
      child = child->GetSibling();
    }
  }
}

int sbChunkWrite( const char* file_name, const sbChunk* chunk )
{
  FILE* file = fopen( file_name, "wb" );
  if( !file ) return 0;
  ChunkWrite( file, chunk );
  fseek( file, 0, SEEK_END );
  int size = ( int )ftell( file );
  fclose( file );
  return size;
}

static sbChunk* GrabChunk( const char** read_ptr, const char* data_end )
{
  sbChunk* chunk = NULL;

  char first_char = **read_ptr;

  uint32_t chunk_id = *( const uint32_t* )*read_ptr;
  *read_ptr += 4;
  uint32_t chunk_size = *( const uint32_t* )*read_ptr;
  *read_ptr += 4;

  if( isupper( first_char ) )
  {
    // This is a container chunk
    chunk = new sbChunk( chunk_id );
    const char* chunk_end = *read_ptr + chunk_size;
    
    while( *read_ptr < chunk_end )
    {
      sbChunk* child = GrabChunk( read_ptr, chunk_end );
      chunk->AddField( child );
      *read_ptr += ( -( intptr_t )*read_ptr ) & 3;
    }
  }
  else
  {
    // This is a leaf chunk
    chunk = new sbChunk( chunk_id, *read_ptr, chunk_size );
    *read_ptr += chunk_size;
  }

  return chunk;
}

const sbChunk* sbChunkRead( const char* file_name, char* buffer, int buffer_size )
{
  sbChunk* chunk = NULL;
  FILE* file = fopen( file_name, "rb" );
  if( file )
  {
    fseek( file, 0, SEEK_END );
    int file_size = ( int )ftell( file );
    fseek( file, 0, SEEK_SET );
    if( file_size <= buffer_size )
    {
      fread( buffer, 1, file_size, file );
      const char* read = buffer;
      chunk = GrabChunk( &read, buffer + file_size );
    }
    fclose( file );
  }
  return chunk;
}

