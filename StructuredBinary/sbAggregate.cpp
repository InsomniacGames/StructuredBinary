//
//  sbAggregate.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//


#include "sbAggregate.h"

#include <stdio.h>
#include <assert.h>
#include <cstring>

#include "sbUtil.h"
#include "sbSchema.h"
#include "sbScalar.h"
#include "sbValue.h"
#include "sbStatus.h"

#include "sbElementField.h"
#include "sbStringPointerField.h"
#include "sbCountedPointerField.h"

const sbField* sbAggregate::FindField( sbHash field_name ) const
{
  return m_Dictionary.FindByName( field_name );
}

sbStatus sbAggregate::Convert( char* dst_element_data, const char* src_element_data, const sbElement* src_element, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    const sbField* dst_field = m_Dictionary.GetByIndex( i );
    sbHash dst_field_name = m_Dictionary.GetNameByIndex( i );
    const sbField* src_field = src_element->FindField( dst_field_name );
    dst_field->Convert( dst_element_data, src_element_data, src_field, alloc );
  }
  return status;
}

bool sbAggregate::IsTerminal( const char* element_data, const sbValue& terminator_value, sbHash terminator_name ) const
{
  const sbField* field = FindField( terminator_name );
  assert( field );
  return field->ReadValue( element_data ) == terminator_value;
}

sbStatus sbAggregate::FixUp( sbSchema* schema )
{
  sbStatus status = sbStatus_Ok;

  if( m_State == kState_Ready ) return status;
  assert( m_State == kState_Defined );
  
  m_State = kState_Fixing;

  size_t alignment = 1;
  const sbField* previous_field = NULL;
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    sbField* field = m_Dictionary.GetByIndex( i );
    status = field->FixUp( schema, previous_field );

    if( status != sbStatus_Ok )
    {
      break;
    }

    size_t field_alignment = 0;
    field_alignment = field->GetAlignment();
    alignment = field_alignment > alignment ? field_alignment : alignment;

    previous_field = field;
  }

  size_t size = previous_field->GetOffset() + previous_field->GetTotalSize();

  m_Size = FIX_ALIGNMENT( size, alignment );
  m_Alignment = alignment;

  m_State = kState_Ready;

  return status;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

void sbAggregate::AddField( sbHash field_name, sbField* field )
{
  m_Dictionary.Add( field_name, field );
}

void sbAggregate::AddInstance( sbHash field_name, int count, sbHash element_name )
{
  AddField( field_name, new sbElementField( this, count, element_name ) );
}

void sbAggregate::AddPointer( sbHash field_name, int count, sbHash element_name, sbHash count_name )
{
  AddField( field_name, new sbCountedPointerField( this, count, element_name, count_name ) );
}

void sbAggregate::AddString( sbHash field_name, int count, sbHash element_name, sbHash terminator_name, const sbValue& terminator_value )
{
  AddField( field_name, new sbStringPointerField( this, count, element_name, terminator_name, terminator_value ) );
}

sbAggregate::~sbAggregate()
{
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    delete m_Dictionary.GetByIndex( i );
  }
}

sbValue sbAggregate::ReadValue( const char* data ) const
{
  assert( false );  
}
