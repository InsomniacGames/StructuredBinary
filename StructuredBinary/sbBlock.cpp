//
//  sbBlock.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/27/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbBlock.h"
#include "sbAllocator.h"

char* sbBlock::GetDstPtr() const
{
  if( !m_Allocator->m_Data ) return NULL;
  if( m_Offset > m_Allocator->m_Size ) return NULL;
  return m_Allocator->m_Data + m_Offset;
}
