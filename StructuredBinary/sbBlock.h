//
//  sbBlock.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/27/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbBlock_h
#define StructuredBinary_sbBlock_h

#include <stddef.h>

class sbType;
class sbAllocator;

struct sbBlock
{
  sbBlock( const sbType* dst_type, const sbType* src_type, const char* src_data, const sbAllocator* alloc, int count )
  : m_DstType( dst_type )
  , m_SrcType( src_type )
  , m_SrcData( src_data )
  , m_Allocator( alloc )
  , m_Count( count )
  , m_Offset( 0 )
  {}
  
  sbBlock()
  : m_DstType( NULL )
  , m_SrcType( NULL )
  , m_SrcData( NULL )
  , m_Allocator( NULL )
  , m_Count( 0 )
  , m_Offset( 0 )
  {}
  
  bool operator == ( const sbBlock& other )
  {
    return  m_DstType == other.m_DstType &&
    m_SrcType == other.m_SrcType &&
    m_SrcData == other.m_SrcData &&
    m_Count   == other.m_Count;
  }

  char* GetDstPtr() const;

  const sbType*       m_DstType;
  const sbType*       m_SrcType;
  const char*         m_SrcData;
  const sbAllocator*  m_Allocator;
  int                 m_Count;
  size_t              m_Offset;
};


#endif
