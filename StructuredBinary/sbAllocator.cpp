//
//  sbAllocator.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/10/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbAllocator.h"

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

sbAllocator::Result sbAllocator::Alloc( size_t element_size, size_t element_count, size_t alignment, const char* src_data )
{  
  char* p = NULL;
  bool done = true;

  Entry* entry = FindEntry( element_size, element_count, alignment, src_data );
  if( !entry )
  {
    done = false;
    entry = AddEntry( element_size, element_count, alignment, src_data );
  }

  if( m_Data && m_Offset <= m_Size )
  {
    p = m_Data + entry->m_Offset;
  }

  Result result;
  result.m_Data = p;
  result.m_Done = done;
  return result;
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

sbAllocator::Entry* sbAllocator::AddEntry( size_t element_size, size_t element_count, size_t alignment, const char* src_data )
{
  Entry* entry = m_Entries + m_EntryCount++;

  entry->m_SrcData      = src_data;
  entry->m_ElementSize  = element_size;
  entry->m_ElementCount = element_count;
  entry->m_Alignment    = alignment;

  m_Offset += -( m_Offset ) & ( alignment - 1 );
  entry->m_Offset       = m_Offset;
  m_Offset += element_size * element_count;

  return entry;
}

sbAllocator::Entry* sbAllocator::FindEntry( size_t element_size, size_t element_count, size_t alignment, const char* src_data )
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    Entry* entry = m_Entries + i;
    if( entry->m_SrcData == src_data && entry->m_ElementSize == element_size && entry->m_ElementCount == element_count && entry->m_Alignment == alignment )
    {
      return entry;
    }
  }
  return NULL;
}

