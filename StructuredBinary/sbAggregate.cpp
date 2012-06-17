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

#include "sbSchema.h"
#include "sbScalar.h"
#include "sbValue.h"
#include "sbStatus.h"

#include "sbElementField.h"
#include "sbStringPointerField.h"
#include "sbCountedPointerField.h"

const sbField* sbAggregate::FindField( sbHash field_name ) const
{
  const sbField* child = NULL;
  if( field_name )
  {
    child = m_FirstField;
    while( child && child->m_FieldName != field_name )
    {
      child = child->m_Next;
    }
  }
  return child;
}

sbStatus sbAggregate::Convert( char* dst_element_data, const char* src_element_data, const sbElement* src_element, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;
  const sbField* child = m_FirstField;
  while( child )
  {
    const sbField* src_child = src_element->FindField( child->m_FieldName );
    child->Convert( dst_element_data, src_element_data, src_child, alloc );
    child = child->m_Next;
  }
  return status;
}

bool sbAggregate::IsTerminal( const char* element_data, const sbValue& terminator_value, sbHash terminator_name ) const
{
  const sbField* child = FindField( terminator_name );
  assert( child );
  return child->m_Element->ReadValue( element_data + child->m_Offset ) == terminator_value;
}

static size_t FixAlignment( size_t value, size_t alignment )
{
  return value + ( -( value ) & ( alignment - 1 ) );
}

sbStatus sbAggregate::FixUp( sbSchema* schema )
{
  sbStatus status = sbStatus_Ok;

  if( m_State == kState_Ready ) return status;
  assert( m_State == kState_Defined );

  size_t offset = 0;
  size_t alignment = 1;

  sbField* child = m_FirstField;
  while( child )
  {
    size_t child_size = 0;
    size_t child_alignment = 0;

    sbElement* element = schema->FindElement( child->m_ElementName );
    if( !element )
    {
      status = sbStatus_ErrorNodeNotFound;
    }
    if( status == sbStatus_Ok )
    {
      child->m_Element = element;
      status = schema->FixUp( child->m_ElementName );
    }
    if( status == sbStatus_Ok )
    {
      child_size = child->GetSize();
      child_alignment = child->GetAlignment();
    }
    if( status == sbStatus_Ok )
    {
      offset = FixAlignment( offset, child_alignment );
      child->m_Offset = offset;
      offset += child_size * child->m_Count;
      alignment = child_alignment > alignment ? child_alignment : alignment;
    }

    if( status != sbStatus_Ok )
    {
      break;
    }
    child = child->m_Next;
  }

  m_Size = FixAlignment( offset, alignment );
  m_Alignment = alignment;
  
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
