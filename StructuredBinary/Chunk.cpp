//
//  Chunk.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "Chunk.h"
  
Chunk::~Chunk()
{
  Chunk* child = m_Child;
  while( child )
  {
    Chunk* temp = child->m_Sibling;
    delete child;
    child = temp;
  }
}

Chunk* Chunk::AddChunk( const char* name )
{
  Chunk* chunk = new Chunk( name );
  if( !m_Child )
  {
    m_Child = chunk;
  }
  else
  {
    Chunk* child = m_Child;
    while( child->m_Sibling )
    {
      child = child->m_Sibling;
    }
    child->m_Sibling = chunk;
  }
  m_ChildCount += 1;
  return chunk;
}

void Chunk::AddLeafChunk( const char* name, const void* data, uint32_t data_size )
{
  Chunk* chunk = new Chunk( name, data, data_size );
  if( !m_Child )
  {
    m_Child = chunk;
  }
  else
  {
    Chunk* child = m_Child;
    while( child->m_Sibling )
    {
      child = child->m_Sibling;
    }
    child->m_Sibling = chunk;
  }
  m_ChildCount += 1;
}
