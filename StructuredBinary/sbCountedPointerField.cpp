//
//  sbCountedPointerField.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbCountedPointerField.h"

#include "sbAggregate.h"

sbCountedPointerField::sbCountedPointerField( const sbAggregate* parent, sbHash field_name, int count, sbHash element_name, sbHash count_name )
: sbPointerField( parent, field_name, count, element_name )
{
  m_CountName = count_name;
}

int sbCountedPointerField::GetPointerCount( const char* aggregate_data, int index ) const
{
  int pointer_count = 1;
  const sbField* count_field = m_Aggregate->FindField( m_CountName );
  if( count_field )
  {
    const char* count_field_data = count_field->GetDataPtr( aggregate_data, 0 );
    pointer_count = ( int )count_field->m_Element->ReadValue( count_field_data ).AsInt();
  }
  return pointer_count;
}
