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

void sbSchema::FixUp()
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    m_Entries[ i ].m_Node->FixUp( this );
  }
}
