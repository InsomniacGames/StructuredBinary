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

#include "sbNode.h"
#include "sbScalarValue.h"
#include "sbStatus.h"

const sbNode* sbSchema::FindNode( const char* name ) const
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    if( 0 == strcmp( m_Entries[ i ].m_Name, name ) )
    {
      return m_Entries[ i ].m_Node;
    }
  }
  return NULL;
}

sbNode* sbSchema::FindNode( const char* name )
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    if( 0 == strcmp( m_Entries[ i ].m_Name, name ) )
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

sbStatus sbSchema::Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, const char* name, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;

  const sbNode* src_node = src_schema->FindNode( name );
  const sbNode* dst_node =             FindNode( name );
  
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

void sbSchema::BeginNode( const char* name )
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

  m_CurrentName = NULL;
  m_CurrentNode = NULL;
}

void sbSchema::AddScalar( const char* name, int count, sbScalarType scalar_type )
{
  assert( m_CurrentNode );
  m_CurrentNode->AddScalar( name, count, scalar_type );
}

void sbSchema::AddInstance( const char* name, int count, const char* link_name )
{
  assert( m_CurrentNode );
  m_CurrentNode->AddInstance( name, count, link_name );
}

void sbSchema::AddPointer( const char* name, int count, const char* link_name, const char* count_name )
{
  assert( m_CurrentNode );
  m_CurrentNode->AddPointer( name, count, link_name, count_name );
}

void sbSchema::AddString( const char* name, int count, const char* link_name, const sbScalarValue& terminator, const char* terminator_name )
{
  assert( m_CurrentNode );
  m_CurrentNode->AddString( name, count, link_name, terminator, terminator_name );
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

