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

#include "sbBlock.h"

class sbAllocator
{
public:
  
  sbAllocator();
  sbAllocator( char* data, size_t size );
  void SetBuffer( char* data, size_t size );

  char* Alloc( const sbType* dst_type, const sbType* src_type, const char* src_data, int count );
  size_t GetSize() const { return m_Offset; }
  void StorePointerLocation( const char* data );
  
  int GetPointerLocationCount() const;
  const char* GetPointerLocation( int index ) const;
  
  int GetCount() const { return m_BlockCount; }

  const sbBlock* GetBlock( int index ) const
  {
    return m_Blocks + index;
  }

//private:
  static const int  kMaxEntries = 100;

  sbBlock m_Blocks[ kMaxEntries ];
  int     m_BlockCount;
  
  const char* m_Pointers[ kMaxEntries ];
  int         m_PointerCount;
  
  char*   m_Data;
  size_t  m_Offset;
  size_t  m_Size;

  sbBlock* FindBlock( const sbType* dst_type, const sbType* src_type, const char* src_data, int count );
  sbBlock* AddBlock( const sbType* dst_type, const sbType* src_type, const char* src_data, int count );
};

#endif
