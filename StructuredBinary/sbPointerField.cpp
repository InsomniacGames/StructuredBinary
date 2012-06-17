//
//  sbPointerField.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbPointerField.h"

#include "sbUtil.h"
#include "sbElement.h"
#include "sbAllocator.h"

#include "sbStatus.h"
#include "sbSchema.h"

sbPointerField::sbPointerField( const sbAggregate* aggregate, int count, sbHash element_name )
: sbField( aggregate, count, element_name )
{}

size_t sbPointerField::GetSize() const
{
  return sizeof( void* );
}

size_t sbPointerField::GetAlignment() const
{
  return __alignof( void* );
}

void sbPointerField::Convert( char* dst_aggregate_data, const char* src_aggregate_data, const sbField* src_field, sbAllocator* alloc ) const
{
  for( int index = 0; index < m_Count; ++index )
  {
    char* dst_field_data = GetDataPtr( dst_aggregate_data, index );
    const char* src_field_data = src_field->GetDataPtr( src_aggregate_data, index );
    int element_count = src_field->GetPointerCount( src_aggregate_data, index );
    
    size_t dst_size = m_Element->GetSize();
    size_t src_size = src_field->m_Element->GetSize();
    
    const char* src_p = *( const char** )( src_field_data );
    sbAllocator::Result r = alloc->Alloc( dst_size, element_count, m_Element->GetAlignment(), src_p );
    char* dst_p = r.m_Data;
    if( dst_field_data )
    {
      *( char** )( dst_field_data ) = dst_p;
    }
    
    if( !r.m_Done )
    {
      for( int j = 0; j < element_count; ++j )
      {
        m_Element->Convert( dst_p, src_p, src_field->m_Element, alloc );
        src_p += src_size;
        dst_p += dst_p ? dst_size : 0;
      }
    }
  }
}

int sbPointerField::GetPointerCount( const char* aggregate_data, int index ) const
{
  return 1;
}

sbStatus sbPointerField::FixUp( sbSchema* schema, const sbField* previous_field )
{
  sbStatus status = sbStatus_Ok;
  
  if( status == sbStatus_Ok )
  {
    m_Element = schema->FindElement( m_ElementName );
    if( !m_Element )
    {
      status = sbStatus_ErrorNodeNotFound;
    }
  }
  
  if( status == sbStatus_Ok )
  {
    size_t offset = previous_field ? previous_field->m_Offset + previous_field->GetTotalSize() : 0;
    offset = FIX_ALIGNMENT( offset, GetAlignment() );
    m_Offset = offset;
  }
  
  return status;
}
