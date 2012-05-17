//
//  UnitTestChunk.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "UnitTestChunk.h"

// Libraries
#include <cstring>
#include <stdio.h>
#include <stdint.h>

// Project
#include "Chunk.h"


const char* UnitTestChunk::RunTest() const
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

  const Chunk* c = &chunk;
  if( c->GetChildCount() != 3 )           return "Top chunk wrong child count";
  if( c->GetDataSize() != 0 )             return "Top chunk wrong data size";
  c = c->GetChild();
  if( c->GetChildCount() != 0 )           return "First child wrong child count";
  if( c->GetDataSize() != sweet_size )    return "First child wrong data size";
  if( 0 != strcmp( ( const char* )c->GetData(), sweet ) )  return "First child wrong data";
  c = c->GetSibling();
  if( c->GetChildCount() != 0 )         return "Second child wrong child count";
  if( c->GetDataSize() != salt_size )   return "Second child wrong data size";
  if( 0 != strcmp( ( const char* )c->GetData(), salt ) )  return "Second child wrong data";
  c = c->GetSibling();
  if( c->GetChildCount() != 3 )         return "Third child wrong child count";
  if( c->GetDataSize() != 0 )           return "Third child wrong data size";
  const Chunk* temp = c->GetSibling();
  if( temp != NULL )                    return "Child list not properly terminated";

  c = c->GetChild();
  if( c->GetChildCount() != 0 )         return "First grand child wrong child count";
  if( c->GetDataSize() != sour_size )   return "First grand child wrong data size";
  c = c->GetSibling();
  if( c->GetChildCount() != 0 )         return "Second grand child wrong child count";
  if( c->GetDataSize() != bitter_size ) return "Second grand child wrong data size";
  c = c->GetSibling();
  if( c->GetChildCount() != 0 )         return "Third grand child wrong child count";
  if( c->GetDataSize() != umami_size )  return "Third grand child wrong data size";
  temp = c->GetSibling();
  if( temp != NULL )                    return "Grand child list not properly terminated";

  return NULL;
}
