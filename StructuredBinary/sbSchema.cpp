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

static const char* GetNameForScalar( sbScalarType t )
{
  switch( t )
  {
    case kScalar_Null:  return "null";
    case kScalar_U8:    return "u8";
    case kScalar_I8:    return "i8";
    case kScalar_U16:   return "u16";
    case kScalar_I16:   return "i16";
    case kScalar_U32:   return "u32";
    case kScalar_I32:   return "i32";
    case kScalar_U64:   return "u64";
    case kScalar_I64:   return "i64";
    case kScalar_F32:   return "f32";
    case kScalar_F64:   return "f64";
  }
}

const sbNode* sbSchema::FindNode( sbHash name ) const
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    if( m_Entries[ i ].m_Name == name )
    {
      return m_Entries[ i ].m_Node;
    }
  }
  return NULL;
}

sbNode* sbSchema::FindNode( sbHash name )
{
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

void sbSchema::AddScalar( sbHash name, int count, sbScalarType scalar_type )
{
  assert( m_CurrentNode );
  m_CurrentNode->AddScalar( name, count, scalar_type );
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

void sbSchema::AddPointer( sbHash name, int count, sbScalarType t, sbHash count_name )
{
  AddPointer( name, count, GetNameForScalar( t ), count_name );
}

void sbSchema::AddString( sbHash name, int count, sbScalarType t, const sbScalarValue& terminator_value )
{
  AddString( name, count, GetNameForScalar( t ), "value", terminator_value );
}


void sbSchema::Begin()
{
  assert( m_State == kState_New );
  m_State = kState_Building;

  // This hack must go away!
  BeginNode( "i8" );
  AddScalar( "value", 1, kScalar_I8  );
  EndNode();
  BeginNode( "u8" );
  AddScalar( "value", 1, kScalar_U8  );
  EndNode();
  BeginNode( "i16" );
  AddScalar( "value", 1, kScalar_I16 );
  EndNode();
  BeginNode( "u16" );
  AddScalar( "value", 1, kScalar_U16 );
  EndNode();
  BeginNode( "i32" );
  AddScalar( "value", 1, kScalar_I32 );
  EndNode();
  BeginNode( "u32" );
  AddScalar( "value", 1, kScalar_U32 );
  EndNode();
  BeginNode( "i64" );
  AddScalar( "value", 1, kScalar_I64 );
  EndNode();
  BeginNode( "u64" );
  AddScalar( "value", 1, kScalar_U64 );
  EndNode();
  BeginNode( "f64" );
  AddScalar( "value", 1, kScalar_F64 );
  EndNode();
  BeginNode( "f32" );
  AddScalar( "value", 1, kScalar_F32 );
  EndNode();
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

