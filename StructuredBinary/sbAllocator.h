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

#include "sbStatus.h"

class sbType;

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

  void ConvertAll();

private:
  static const int  kMaxEntries = 100;

  struct Block
  {
    const sbType*       m_DstType;
    const sbType*       m_SrcType;
    const char*         m_SrcData;
    int                 m_Count;
    size_t              m_Offset;
  };

  Block m_Blocks[ kMaxEntries ];
  int     m_BlockCount;
  
  const char* m_Pointers[ kMaxEntries ];
  int         m_PointerCount;
  
  char*   m_Data;
  size_t  m_Offset;
  size_t  m_Size;

  Block* FindBlock( const sbType* dst_type, const sbType* src_type, const char* src_data, int count );
  Block* AddBlock( const sbType* dst_type, const sbType* src_type, const char* src_data, int count );

  const Block* GetBlock( int index ) const
  {
    return m_Blocks + index;
  }
  
  sbStatus ConvertMany( const Block* block );

  char* GetDstPtr( const Block* block ) const;
};

#endif
