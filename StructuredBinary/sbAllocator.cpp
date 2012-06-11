//
//  sbAllocator.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/10/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbAllocator.h"

sbAllocator::sbAllocator( char* base, size_t size )
{
  m_Base = base;
  m_Size = size;
  m_Offset = 0;
}

char* sbAllocator::Alloc( size_t size, size_t alignment, const char* src_data )
{
  m_Offset += -( m_Offset ) & ( alignment - 1 );
  char* p = m_Base + m_Offset;
  m_Offset += size;
  return p;
}
