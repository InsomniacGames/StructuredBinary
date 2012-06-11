//
//  sbAllocator.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/10/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbAllocator_h
#define StructuredBinary_sbAllocator_h

#include <stddef.h>

class sbAllocator
{
public:
  sbAllocator( char* base, size_t size );

  char* Alloc( size_t size, size_t alignment, const char* src_data );
  size_t GetSize() const { return m_Offset; }

private:
  char*   m_Base;
  size_t  m_Offset;
  size_t  m_Size;
};

#endif
