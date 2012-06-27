//
//  sbAllocator.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/10/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbAllocator.h"
#include "sbBlock.h"
#include "sbType.h"

sbAllocator::sbAllocator( char* data, size_t size )
{
  SetBuffer( data, size );
}

sbAllocator::sbAllocator()
{
  SetBuffer( NULL, 0 );
}

void sbAllocator::SetBuffer( char* data, size_t size )
{
  m_Data = data;
  m_Size = size;
  m_Offset = 0;
  m_BlockCount = 0;
  m_PointerCount = 0;
}

char* sbAllocator::Alloc( const sbType* dst_type, const sbType* src_type, const char* src_data, int count )
{
  char* p = NULL;

  sbBlock* block = FindBlock( dst_type, src_type, src_data, count );
  if( !block )
  {
    block = AddBlock( dst_type, src_type, src_data, count );
  }

  if( m_Data && m_Offset <= m_Size )
  {
    p = m_Data + block->m_Offset;
  }

  return p;
}

void sbAllocator::StorePointerLocation( const char* data )
{
  m_Pointers[ m_PointerCount++ ] = data;
}

int sbAllocator::GetPointerLocationCount() const
{
  return m_PointerCount;
}

const char* sbAllocator::GetPointerLocation( int index ) const
{
  return m_Pointers[ index ];
}

sbBlock* sbAllocator::AddBlock( const sbType* dst_type, const sbType* src_type, const char* src_data, int count )
{
  sbBlock* block = m_Blocks + m_BlockCount++;

  block->m_Allocator = this;
  block->m_DstType = dst_type;
  block->m_SrcType = src_type;
  block->m_SrcData = src_data;
  block->m_Count = count;

  m_Offset += -( m_Offset ) & ( dst_type->GetAlignment() - 1 );
  block->m_Offset = m_Offset;
  m_Offset += dst_type->GetSize() * count;

  return block;
}

sbBlock* sbAllocator::FindBlock( const sbType* dst_type, const sbType* src_type, const char* src_data, int count )
{
  for( int i = 0; i < m_BlockCount; ++i )
  {
    sbBlock* block = m_Blocks + i;
    if(
       block->m_DstType == dst_type &&
       block->m_SrcType == src_type &&
       block->m_SrcData == src_data &&
       block->m_Count == count
      )
    {
      return block;
    }
  }
  return NULL;
}
