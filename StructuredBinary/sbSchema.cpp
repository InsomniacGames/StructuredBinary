//
//  sbSchema.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbSchema.h"

#include <cstring>

#include "sbNode.h"
#include "sbPath.h"
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

void sbSchema::AddNode( const char* name, sbNode* node )
{
  Entry* entry = m_Entries + m_EntryCount++;
  entry->m_Name = name;
  entry->m_Node = node;
}

void sbSchema::PrintNode( const char* node_name, const char* data ) const
{
  sbPath path( NULL );
  path.m_Name = node_name;
  const sbNode* node = FindNode( node_name );
  node->PrintNode( data, &path );
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

sbStatus sbSchema::Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, const char* name ) const
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
    status = dst_node->Convert( dst_data, src_data, src_node );
  }
  return status;
}
