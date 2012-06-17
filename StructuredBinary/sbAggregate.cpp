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
  const sbField* field = NULL;
  if( field_name )
  {
    field = m_FirstField;
    while( field && field->GetName() != field_name )
    {
      field = field->GetNext();
    }
  }
  return field;
}

sbStatus sbAggregate::Convert( char* dst_element_data, const char* src_element_data, const sbElement* src_element, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;
  const sbField* dst_field = m_FirstField;
  while( dst_field )
  {
    const sbField* src_field = src_element->FindField( dst_field->GetName() );
    dst_field->Convert( dst_element_data, src_element_data, src_field, alloc );
    dst_field = dst_field->GetNext();
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

  const sbField* previous_field = NULL;
  sbField* field = m_FirstField;
  while( field )
  {
    status = field->FixUp( schema, previous_field );

    if( status != sbStatus_Ok )
    {
      break;
    }

    previous_field = field;
    field = field->GetNext();
  }

  //-----------------------------------------------------------
  size_t alignment = 1;
  field = m_FirstField;
  while( field )
  {
    size_t field_alignment = 0;
    field_alignment = field->GetAlignment();
    alignment = field_alignment > alignment ? field_alignment : alignment;

    field = field->GetNext();
  }
  
  size_t size = m_LastField->m_Offset + m_LastField->GetTotalSize();

  m_Size = FIX_ALIGNMENT( size, alignment );
  m_Alignment = alignment;

  m_State = kState_Ready;

  return status;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

void sbAggregate::AddField( sbField* child )
{
  if( m_LastField )
  {
    m_LastField->m_Next = child;
  }
  m_LastField = child;
  if( !m_FirstField )
  {
    m_FirstField = child;
  }
}

void sbAggregate::AddInstance( sbHash name, int count, sbHash element_name )
{
  AddField( new sbElementField( this, name, count, element_name ) );
}

void sbAggregate::AddPointer( sbHash name, int count, sbHash element_name, sbHash count_name )
{
  AddField( new sbCountedPointerField( this, name, count, element_name, count_name ) );
}

void sbAggregate::AddString( sbHash name, int count, sbHash element_name, sbHash terminator_name, const sbValue& terminator_value )
{
  AddField( new sbStringPointerField( this, name, count, element_name, terminator_name, terminator_value ) );
}

sbAggregate::~sbAggregate()
{
  const sbField* child = m_FirstField;
  while( child )
  {
    const sbField* next = child->m_Next;
    delete child;
    child = next;
  }
}

sbValue sbAggregate::ReadValue( const char* data ) const
{
  assert( false );  
}
