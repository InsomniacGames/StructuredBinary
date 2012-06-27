//
//  sbAllocator.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/10/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbAllocator.h"
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
  m_EntryCount = 0;
  m_PointerCount = 0;
}

char* sbAllocator::Alloc( const sbType* dst_type, const sbType* src_type, const char* src_data, int count )
{
  sbBlock block( dst_type, src_type, src_data, count );

  char* p = NULL;

  Entry* entry = FindEntry( block );
  if( !entry )
  {
    entry = AddEntry( block );
  }

  if( m_Data && m_Offset <= m_Size )
  {
    p = m_Data + entry->m_Offset;
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

sbAllocator::Entry* sbAllocator::AddEntry( const sbBlock& block )
{
  Entry* entry = m_Entries + m_EntryCount++;

  entry->m_Block        = block;

  m_Offset += -( m_Offset ) & ( block.m_DstType->GetAlignment() - 1 );
  entry->m_Offset = m_Offset;
  m_Offset += block.m_DstType->GetSize() * block.m_Count;

  return entry;
}

sbAllocator::Entry* sbAllocator::FindEntry( const sbBlock& block )
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    Entry* entry = m_Entries + i;
    if( entry->m_Block == block )
    {
      return entry;
    }
  }
  return NULL;
}
