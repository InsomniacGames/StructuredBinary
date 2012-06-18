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
#include "sbAggregateType.h"
#include "sbValue.h"
#include "sbStatus.h"

#include "sbFloatType.h"
#include "sbIntType.h"

#include "sbDictionary.h"

const sbType* sbSchema::FindType( sbHash name ) const
{
  return m_Dictionary.FindByName( name );
}

sbType* sbSchema::FindType( sbHash name )
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

sbStatus sbSchema::FixUp( sbHash type_name )
{
  sbStatus status = sbStatus_Ok;
  sbType* type_def = m_Dictionary.FindByName( type_name );
  if( !type_name )
  {
    status = sbStatus_ErrorNodeNotFound;
  }
  else
  {
    status = type_def->FixUp( this );
  }
  return status;
}

sbStatus sbSchema::Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, sbHash name, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;

  const sbType* src_type = src_schema->FindType( name );
  const sbType* dst_type =             FindType( name );
  
  if( !src_type || !dst_type )
  {
    status = sbStatus_ErrorNodeNotFound;
  }
  else
  {
    status = dst_type->Convert( dst_data, src_data, src_type, alloc );
  }
  return status;
}

void sbSchema::BeginType( sbHash type_name )
{
  assert( !m_CurrentAggregate );
  
  m_CurrentName = type_name;
  m_CurrentAggregate = new sbAggregateType();
}

void sbSchema::EndType()
{
  m_Dictionary.Add( m_CurrentName, m_CurrentAggregate );
  m_CurrentName = 0U;
  m_CurrentAggregate = NULL;
}

void sbSchema::AddInstance( sbHash member_name, int count, sbHash type_name )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddInstance( member_name, count, type_name );
}

void sbSchema::AddPointer( sbHash member_name, int count, sbHash type_name )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddPointer( member_name, count, type_name );
}

void sbSchema::AddCountedPointer( sbHash member_name, int count, sbHash type_name, sbHash count_name )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddCountedPointer( member_name, count, type_name, count_name );
}

void sbSchema::AddStringPointer( sbHash member_name, int count, sbHash type_name, sbHash terminator_name, const sbValue& terminator_value )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddStringPointer( member_name, count, type_name, terminator_name, terminator_value );
}

void sbSchema::Begin()
{
  assert( m_State == kState_New );
  m_State = kState_Building;

  m_Dictionary.Add( "uint8_t",   new sbIntType<  uint8_t > );
  m_Dictionary.Add( "int8_t",    new sbIntType<   int8_t > );
  m_Dictionary.Add( "uint16_t",  new sbIntType< uint16_t > );
  m_Dictionary.Add( "int16_t",   new sbIntType<  int16_t > );
  m_Dictionary.Add( "uint32_t",  new sbIntType< uint32_t > );
  m_Dictionary.Add( "int32_t",   new sbIntType<  int32_t > );
  m_Dictionary.Add( "uint64_t",  new sbIntType< uint64_t > );
  m_Dictionary.Add( "int64_t",   new sbIntType<  int64_t > );
  m_Dictionary.Add( "float",     new sbFloatType<  float > );
  m_Dictionary.Add( "double",    new sbFloatType< double > );
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


