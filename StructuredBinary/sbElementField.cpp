//
//  sbElementField.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbElementField.h"
#include "sbUtil.h"
#include "sbElement.h"
#include "sbField.h"
#include "sbSchema.h"

sbElementField::sbElementField( const sbAggregate* aggregate, sbHash field_name, int count, sbHash element_name )
: sbField( aggregate, field_name, count, element_name )
{}


size_t sbElementField::GetSize() const
{
  return m_Element->GetSize();
}

size_t sbElementField::GetAlignment() const
{
  return m_Element->GetAlignment();
}

int sbElementField::GetPointerCount( const char* aggregate_data, int index ) const
{
  return 0;
}

void sbElementField::Convert( char* dst_aggregate_data, const char* src_aggregate_data, const sbField* src_field, sbAllocator* alloc ) const
{
  for( int index = 0; index < m_Count; ++index )
  {
    char* dst_field_data = GetDataPtr( dst_aggregate_data, index );
    const char* src_field_data = src_field->GetDataPtr( src_aggregate_data, index );
    m_Element->Convert( dst_field_data, src_field_data, src_field->m_Element, alloc );
  }
}

sbStatus sbElementField::FixUp( sbSchema* schema, const sbField* previous_field )
{
  sbStatus status = schema->FixUp( m_ElementName );

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
