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


static sbIntScalar<  uint8_t >  s_ScalarU8;
static sbIntScalar<   int8_t >  s_ScalarI8;
static sbIntScalar< uint16_t >  s_ScalarU16;
static sbIntScalar<  int16_t >  s_ScalarI16;
static sbIntScalar< uint32_t >  s_ScalarU32;
static sbIntScalar<  int32_t >  s_ScalarI32;
static sbIntScalar< uint64_t >  s_ScalarU64;
static sbIntScalar<  int64_t >  s_ScalarI64;
static sbFloatScalar<  float >  s_ScalarF32;
static sbFloatScalar< double >  s_ScalarF64;

const sbSchema::Entry sbSchema::s_ScalarEntries[] =
{
  Entry( "uint8_t",   &s_ScalarU8  ),
  Entry( "int8_t",    &s_ScalarI8  ),
  Entry( "uint16_t",  &s_ScalarU16 ),
  Entry( "int16_t",   &s_ScalarI16 ),
  Entry( "uint32_t",  &s_ScalarU32 ),
  Entry( "int32_t",   &s_ScalarI32 ),
  Entry( "uint64_t",  &s_ScalarU64 ),
  Entry( "int64_t",   &s_ScalarI64 ),
  Entry( "float",     &s_ScalarF32 ),
  Entry( "double",    &s_ScalarF64 ),
};

const sbElement* sbSchema::FindElement( sbHash name ) const
{
  for( int i = 0; i < ARRAY_SIZE( s_ScalarEntries ); ++i )
  {
    if( s_ScalarEntries[ i ].m_Name == name )
    {
      return s_ScalarEntries[ i ].m_Node;
    }
  }

  for( int i = 0; i < m_EntryCount; ++i )
  {
    if( m_Entries[ i ].m_Name == name )
    {
      return m_Entries[ i ].m_Node;
    }
  }
  return NULL;
}

sbElement* sbSchema::FindElement( sbHash name )
{
  for( int i = 0; i < ARRAY_SIZE( s_ScalarEntries ); ++i )
  {
    if( s_ScalarEntries[ i ].m_Name == name )
    {
      return s_ScalarEntries[ i ].m_Node;
    }
  }
  
  for( int i = 0; i < m_EntryCount; ++i )
  {
    if( m_Entries[ i ].m_Name == name )
    {
      return m_Entries[ i ].m_Node;
    }
  }
  return NULL;
}

sbStatus sbSchema::FixUp()
{
  sbStatus status = sbStatus_Ok;
  for( int i = 0; i < m_EntryCount; ++i )
  {
    status = m_Entries[ i ].m_Node->FixUp( this );
    if( status != sbStatus_Ok )
      break;
  }
  return status;
}

sbStatus sbSchema::FixUp( sbHash element_name )
{
  sbStatus status = sbStatus_Ok;
  for( int i = 0; i < m_EntryCount; ++i )
  {
    if( m_Entries[ i ].m_Name == element_name )
    {
      status = m_Entries[ i ].m_Node->FixUp( this );
      break;
    }
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
  Entry* entry = m_Entries + m_EntryCount++;
  entry->m_Name = m_CurrentName;
  entry->m_Node = m_CurrentAggregate;

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
}

void sbSchema::End()
{
  assert( m_State == kState_Building );
  FixUp();
  m_State = kState_Ready;
}

sbSchema::~sbSchema()
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    delete m_Entries[ i ].m_Node;
  }
}

sbSchema::sbSchema()
: m_State( kState_New )
, m_CurrentAggregate( NULL)
, m_EntryCount( 0 )
{}


