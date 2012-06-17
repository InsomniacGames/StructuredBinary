//
//  sbSchema.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbSchema.h"

#include <cstring>
#include <assert.h>

#include "sbUtil.h"
#include "sbAggregate.h"
#include "sbValue.h"
#include "sbStatus.h"

#include "sbFloatScalar.h"
#include "sbIntScalar.h"

#include "sbDictionary.h"

const sbElement* sbSchema::FindElement( sbHash name ) const
{
  return m_Dictionary.FindByName( name );
}

sbElement* sbSchema::FindElement( sbHash name )
{
  return m_Dictionary.FindByName( name );
}

sbStatus sbSchema::FixUp()
{
  sbStatus status = sbStatus_Ok;
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    status = m_Dictionary.GetByIndex( i )->FixUp( this );
    if( status != sbStatus_Ok )
      break;
  }
  return status;
}

sbStatus sbSchema::FixUp( sbHash element_name )
{
  sbStatus status = sbStatus_Ok;
  sbElement* element = m_Dictionary.FindByName( element_name );
  if( !element_name )
  {
    status = sbStatus_ErrorNodeNotFound;
  }
  else
  {
    status = element->FixUp( this );
  }
  return status;
}

sbStatus sbSchema::Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, sbHash name, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;

  const sbElement* src_node = src_schema->FindElement( name );
  const sbElement* dst_node =             FindElement( name );
  
  if( !src_node || !dst_node )
  {
    status = sbStatus_ErrorNodeNotFound;
  }
  else
  {
    status = dst_node->Convert( dst_data, src_data, src_node, alloc );
  }
  return status;
}

void sbSchema::BeginElement( sbHash element_name )
{
  assert( !m_CurrentAggregate );
  
  m_CurrentName = element_name;
  m_CurrentAggregate = new sbAggregate();
}

void sbSchema::EndElement()
{
  m_Dictionary.Add( m_CurrentName, m_CurrentAggregate );
  m_CurrentName = 0U;
  m_CurrentAggregate = NULL;
}

void sbSchema::AddInstance( sbHash field_name, int count, sbHash element_name )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddInstance( field_name, count, element_name );
}

void sbSchema::AddPointer( sbHash field_name, int count, sbHash element_name, sbHash count_name )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddPointer( field_name, count, element_name, count_name );
}

void sbSchema::AddString( sbHash field_name, int count, sbHash element_name, sbHash terminator_name, const sbValue& terminator_value )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddString( field_name, count, element_name, terminator_name, terminator_value );
}

void sbSchema::Begin()
{
  assert( m_State == kState_New );
  m_State = kState_Building;

  m_Dictionary.Add( "uint8_t",   new sbIntScalar<  uint8_t > );
  m_Dictionary.Add( "int8_t",    new sbIntScalar<   int8_t > );
  m_Dictionary.Add( "uint16_t",  new sbIntScalar< uint16_t > );
  m_Dictionary.Add( "int16_t",   new sbIntScalar<  int16_t > );
  m_Dictionary.Add( "uint32_t",  new sbIntScalar< uint32_t > );
  m_Dictionary.Add( "int32_t",   new sbIntScalar<  int32_t > );
  m_Dictionary.Add( "uint64_t",  new sbIntScalar< uint64_t > );
  m_Dictionary.Add( "int64_t",   new sbIntScalar<  int64_t > );
  m_Dictionary.Add( "float",     new sbFloatScalar<  float > );
  m_Dictionary.Add( "double",    new sbFloatScalar< double > );
}

void sbSchema::End()
{
  assert( m_State == kState_Building );
  FixUp();
  m_State = kState_Ready;
}

sbSchema::~sbSchema()
{
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    delete m_Dictionary.GetByIndex( i );
  }
}

sbSchema::sbSchema()
: m_State( kState_New )
, m_CurrentAggregate( NULL)
, m_Dictionary( 16, NULL )
{}


