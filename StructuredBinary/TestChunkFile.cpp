//
//  UnitTestChunkFile.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "TestChunkFile.h"

// Libraries
#include <string.h>
#include <stdio.h>
#include <stdint.h>

// Project
#include "sbChunk.h"
#include "sbChunkFile.h"

static char FileBuffer[ 1000 ];
static const char* s_FileName = "TestChunkFile.chunk";

UnitTest::Result TestChunkFile::RunTest() const
{
  remove( s_FileName );
  FILE* file = fopen( s_FileName, "rb" );
  bool exist = file != NULL;
  if( exist )
  {
    fclose( file );
    return Error( "File %s already exists", s_FileName );
  }
  
  const uint32_t top_id    = CHUNK_ID( 'S', 'T', 'R', 'B' );
  const uint32_t child_id  = CHUNK_ID( 'C', 'H', 'L', 'D' );
  const uint32_t sweet_id  = CHUNK_ID( 's', 'w', 'e', 'e' );
  const uint32_t salt_id   = CHUNK_ID( 's', 'a', 'l', 't' );
  const uint32_t bitter_id = CHUNK_ID( 'b', 't', 't', 'r' );
  const uint32_t sour_id   = CHUNK_ID( 's', 'o', 'u', 'r' );
  const uint32_t umami_id  = CHUNK_ID( 'u', 'm', 'a', 'm' );

  const char* sweet_data  = "Sugar&Honey";
  const char* salt_data   = "Chips&Pretzels";
  const char* bitter_data = "Chocolate&Grapefruit";
  const char* sour_data   = "Lemon&Vinegar";
  const char* umami_data  = "Asparagus&Cheese";
  
  uint32_t sweet_size   = ( uint32_t )strlen( sweet_data  ) + 1;
  uint32_t salt_size    = ( uint32_t )strlen( salt_data   ) + 1;
  uint32_t bitter_size  = ( uint32_t )strlen( bitter_data ) + 1;
  uint32_t sour_size    = ( uint32_t )strlen( sour_data   ) + 1;
  uint32_t umami_size   = ( uint32_t )strlen( umami_data  ) + 1;
  
  sbChunk* write_root = new sbChunk( top_id );
  write_root->AddField( new sbChunk( sweet_id, sweet_data, sweet_size ) );
  write_root->AddField( new sbChunk( salt_id, salt_data, salt_size ) );
  
  sbChunk* child = write_root->AddField( new sbChunk( child_id ) );
  child->AddField( new sbChunk( sour_id, sour_data, sour_size ) );
  child->AddField( new sbChunk( bitter_id, bitter_data, bitter_size ) );
  child->AddField( new sbChunk( umami_id, umami_data, umami_size ) );

  int file_size = sbChunkWrite( s_FileName, write_root );
  if( file_size == 0 ) return Error( "Could not write file" );
  
  delete write_root;

  const sbChunk* read_root = sbChunkRead( s_FileName, FileBuffer, sizeof( FileBuffer ) );
  
  if( !read_root ) return Error( "Failed to read file" );

  const sbChunk* c = read_root;
  if( c->GetChildCount() != 3 )         return Error( "Top chunk wrong child count" );
  if( c->GetDataSize() != 0 )           return Error( "Top chunk wrong data size" );
  c = c->GetChild();
  if( c->GetChildCount() != 0 )         return Error( "First child wrong child count" );
  if( c->GetDataSize() != sweet_size )  return Error( "First child wrong data size" );
  if( 0 != strcmp( ( const char* )c->GetData(), sweet_data ) )  return Error( "First child wrong data" );
  c = c->GetSibling();
  if( c->GetChildCount() != 0 )         return Error( "Second child wrong child count" );
  if( c->GetDataSize() != salt_size )   return Error( "Second child wrong data size" );
  if( 0 != strcmp( ( const char* )c->GetData(), salt_data ) )  return Error( "Second child wrong data" );
  c = c->GetSibling();
  if( c->GetChildCount() != 3 )         return Error( "Third child wrong child count" );
  if( c->GetDataSize() != 0 )           return Error( "Third child wrong data size" );
  const sbChunk* temp = c->GetSibling();
  if( temp != NULL )                    return Error( "Child list not properly terminated" );
  
  c = c->GetChild();
  if( c->GetChildCount() != 0 )         return Error( "First grand child wrong child count" );
  if( c->GetDataSize() != sour_size )   return Error( "First grand child wrong data size" );
  c = c->GetSibling();
  if( c->GetChildCount() != 0 )         return Error( "Second grand child wrong child count" );
  if( c->GetDataSize() != bitter_size ) return Error( "Second grand child wrong data size" );
  c = c->GetSibling();
  if( c->GetChildCount() != 0 )         return Error( "Third grand child wrong child count" );
  if( c->GetDataSize() != umami_size )  return Error( "Third grand child wrong data size" );
  temp = c->GetSibling();
  if( temp != NULL )                    return Error( "Grand child list not properly terminated" );
  
  delete read_root;

  return Ok();
}
