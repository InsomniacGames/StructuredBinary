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

Chunk* Chunk::AddChunk( uint32_t id )
{
  Chunk* chunk = new Chunk( id );
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

void Chunk::AddLeafChunk( uint32_t id, const void* data, uint32_t data_size )
{
  Chunk* chunk = new Chunk( id, data, data_size );
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
