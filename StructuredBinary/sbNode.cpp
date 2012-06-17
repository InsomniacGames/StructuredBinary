//
//  sbNode.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//


#include "sbNode.h"

#include <stdio.h>
#include <assert.h>
#include <cstring>

#include "sbSchema.h"
#include "sbScalar.h"
#include "sbScalarValue.h"
#include "sbStatus.h"

const Child* sbNode::FindChild( sbHash name ) const
{
  const Child* child = NULL;
  if( name )
  {
    for( int i = 0; i < m_ChildCount; ++i )
    {
      if( m_Children[ i ]->m_Name == name )
      {
        child = m_Children[ i ];
      }
    }
  }
  return child;
}

sbStatus sbNode::Convert( char* dst_data, const char* src_data, const sbField* src_node, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;
  for( int i = 0; i < m_ChildCount; ++i )
  {
    m_Children[ i ]->Convert( dst_data, src_data, src_node, alloc );
  }
  return status;
}

bool sbNode::IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const
{
  const Child* child = FindChild( terminator_name );
  assert( child );
  return child->m_Node->IsTerminal( data + child->m_Offset, terminator_value, 0U );
}

int sbNode::GetPointerCount( sbHash count_name, const char* node_data ) const
{
  int pointer_count = 1;
  const Child* count_child = FindChild( count_name );
  if( count_child )
  {
    const char* count_child_data = node_data + count_child->m_Offset;
    pointer_count = ( int )count_child->m_Node->ReadValue( count_child_data ).AsInt();
  }
  return pointer_count;
}

// 

int sbNode::GetStringCount( const sbField* string_node, const sbScalarValue& terminator_value, sbHash terminator_name, const char* child_data ) const
{
  int string_count = 0;
  const char* p = *( const char** )( child_data );
  bool terminated = false;
  while( !terminated )
  {
    string_count += 1;
    terminated = string_node->IsTerminal( p, terminator_value, terminator_name );
    p += string_node->GetSize();
  }
  return string_count;
}

static size_t FixAlignment( size_t value, size_t alignment )
{
  return value + ( -( value ) & ( alignment - 1 ) );
}

sbStatus sbNode::FixUp( sbSchema* schema )
{
  sbStatus status = sbStatus_Ok;

  if( m_State == kState_Ready ) return status;
  assert( m_State == kState_Defined );

  size_t offset = 0;
  size_t alignment = 1;

  for( int i = 0; i < m_ChildCount; ++i )
  {
    Child* child = m_Children[ i ];
    size_t child_size = 0;
    size_t child_alignment = 0;

    sbField* node = schema->FindNode( child->m_LinkName );
    if( !node )
    {
      status = sbStatus_ErrorNodeNotFound;
    }
    if( status == sbStatus_Ok )
    {
      child->m_Node = node;
      status = child->m_Node->FixUp( schema );
    }
    if( status == sbStatus_Ok )
    {
      child_size = child->GetSize();
      child_alignment = child->GetAlignment();
    }
    if( status != sbStatus_Ok )
    {
      break;
    }
    else
    {
      offset = FixAlignment( offset, child_alignment );
      child->m_Offset = offset;
      offset += child_size * child->m_Count;
      alignment = child_alignment > alignment ? child_alignment : alignment;
      child->m_ElementSize = child_size;
      child->m_ElementAlignment = child_alignment;
    }
  }

  m_Size = FixAlignment( offset, alignment );
  m_Alignment = alignment;
  
  return status;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

void sbNode::AddInstance( sbHash name, int count, sbHash link_name )
{
  ChildInstance* child = new ChildInstance();
  m_Children[ m_ChildCount++ ] = child;;
  
  child->m_Name   = name;
  child->m_LinkName = link_name;
  child->m_Offset = 0;          // To be determined in second pass
  child->m_Count  = count;
  child->m_Node   = NULL;
}

void sbNode::AddPointer( sbHash name, int count, sbHash link_name, sbHash count_name )
{
  ChildPointer* child = new ChildPointer();
  m_Children[ m_ChildCount++ ] = child;;
  
  child->m_Name   = name;
  child->m_LinkName = link_name;
  child->m_CountName = count_name;
  child->m_Offset = 0;          // To be determined in second pass
  child->m_Count  = count;
  child->m_Node   = NULL;
}

void sbNode::AddString( sbHash name, int count, sbHash link_name, sbHash terminator_name, const sbScalarValue& terminator_value )
{
  ChildString* child = new ChildString();
  m_Children[ m_ChildCount++ ] = child;;
  
  child->m_Name   = name;
  child->m_LinkName = link_name;
  child->m_Offset = 0;          // To be determined in second pass
  child->m_Count  = count;
  child->m_Node   = NULL;
  child->m_TerminatorValue = terminator_value;
  child->m_TerminatorName = terminator_name;
}

sbNode::~sbNode()
{
  for( int i = 0; i < m_ChildCount; ++i )
  {
    delete m_Children[ i ];
  }
}

sbScalarValue sbNode::ReadValue( const char* data ) const
{
  assert( false );  
}
