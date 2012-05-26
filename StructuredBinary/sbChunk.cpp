//
//  sbChunk.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbChunk.h"
  
sbChunk::~sbChunk()
{
  sbChunk* child = m_Child;
  while( child )
  {
    sbChunk* temp = child->m_Sibling;
    delete child;
    child = temp;
  }
}

sbChunk* sbChunk::AddChild( sbChunk* chunk )
{
  if( !m_Child )
  {
    m_Child = chunk;
  }
  else
  {
    // Append after last child
    sbChunk* child = m_Child;
    while( child->m_Sibling )
    {
      child = child->m_Sibling;
    }
    child->m_Sibling = chunk;
  }
  m_ChildCount += 1;
  return chunk;
}
