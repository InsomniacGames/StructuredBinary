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
#include "sbNode.h"
#include "sbScalarValue.h"
#include "sbStatus.h"


static sbScalarU8   s_ScalarU8;
static sbScalarI8   s_ScalarI8;
static sbScalarU16  s_ScalarU16;
static sbScalarI16  s_ScalarI16;
static sbScalarU32  s_ScalarU32;
static sbScalarI32  s_ScalarI32;
static sbScalarU64  s_ScalarU64;
static sbScalarI64  s_ScalarI64;
static sbScalarF32  s_ScalarF32;
static sbScalarF64  s_ScalarF64;

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

const sbField* sbSchema::FindNode( sbHash name ) const
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

sbField* sbSchema::FindNode( sbHash name )
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

sbStatus sbSchema::Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, sbHash name, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;

  const sbField* src_node = src_schema->FindNode( name );
  const sbField* dst_node =             FindNode( name );
  
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

void sbSchema::BeginNode( sbHash name )
{
  assert( !m_CurrentNode );
  
  m_CurrentName = name;
  m_CurrentNode = new sbNode();
}

void sbSchema::EndNode()
{
  Entry* entry = m_Entries + m_EntryCount++;
  entry->m_Name = m_CurrentName;
  entry->m_Node = m_CurrentNode;

  m_CurrentName = 0U;
  m_CurrentNode = NULL;
}

void sbSchema::AddInstance( sbHash name, int count, sbHash link_name )
{
  assert( m_CurrentNode );
  m_CurrentNode->AddInstance( name, count, link_name );
}

void sbSchema::AddPointer( sbHash name, int count, sbHash link_name, sbHash count_name )
{
  assert( m_CurrentNode );
  m_CurrentNode->AddPointer( name, count, link_name, count_name );
}

void sbSchema::AddString( sbHash name, int count, sbHash link_name, sbHash terminator_name, const sbScalarValue& terminator_value )
{
  assert( m_CurrentNode );
  m_CurrentNode->AddString( name, count, link_name, terminator_name, terminator_value );
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

