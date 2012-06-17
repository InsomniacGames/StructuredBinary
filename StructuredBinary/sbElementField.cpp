//
//  sbElementField.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbElementField.h"
#include "sbElement.h"
#include "sbField.h"

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
