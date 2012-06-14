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

const sbScalar* FindScalar( sbScalarType scalar_type );

const sbNode::Child* sbNode::FindChild( const char* name ) const
{
  for( int i = 0; i < m_ChildCount; ++i )
  {
    if( 0 == strcmp( m_Children[ i ].m_Name, name ) )
    {
      return m_Children + i;
    }
  }
  return NULL;
}

sbStatus sbNode::Convert( char* dst_data, const char* src_data, const sbNode* src_node, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;
  for( int i = 0; i < m_ChildCount; ++i )
  {
    const Child* dst_child = m_Children + i;
    const Child* src_child = src_node->FindChild( dst_child->m_Name );

    for( int index = 0; index < dst_child->m_Count; ++index )
    {
      char* dst_child_data = dst_data ? dst_data + dst_child->m_Offset + index * dst_child->m_ElementSize : NULL;
      const char* src_child_data = src_data + src_child->m_Offset + index * src_child->m_ElementSize;
      
      switch( dst_child->m_Type )
      {
        case Child::kType_Scalar:
        {
          sbScalarValue src_value = src_child->m_Scalar->ReadValue( src_child_data );
          if( dst_child_data )
          {
            dst_child->m_Scalar->WriteValue( dst_child_data, src_value );
          }
          break;
        }
        case Child::kType_Pointer:
        {
          int element_count = src_node->GetPointerCount( src_child, src_data );
          size_t dst_size = dst_child->m_Node->GetSize();
          size_t src_size = src_child->m_Node->GetSize();

          const char* src_p = *( const char** )( src_child_data );
          sbAllocator::Result r = alloc->Alloc( dst_size, element_count, dst_child->m_Node->GetAlignment(), src_p );
          char* dst_p = r.m_Data;
          if( dst_child_data )
          {
            *( char** )( dst_child_data ) = dst_p;
          }

          if( !r.m_Done )
          {
            for( int j = 0; j < element_count; ++j )
            {
              dst_child->m_Node->Convert( dst_p, src_p, src_child->m_Node, alloc );
              src_p += src_size;
              dst_p += dst_p ? dst_size : 0;
            }
          }
          break;
        }
        case Child::kType_String:
        {
          int element_count = src_node->GetStringCount( src_child, src_child_data );
          size_t dst_size = dst_child->m_Node->GetSize();
          size_t src_size = src_child->m_Node->GetSize();
          
          const char* src_p = *( const char** )( src_child_data );
          sbAllocator::Result r = alloc->Alloc( dst_size, element_count, dst_child->m_Node->GetAlignment(), src_p );
          char* dst_p = r.m_Data;
          if( dst_child_data )
          {
            *( char** )( dst_child_data ) = dst_p;
          }
          
          if( !r.m_Done )
          {
            for( int j = 0; j < element_count; ++j )
            {
              dst_child->m_Node->Convert( dst_p, src_p, src_child->m_Node, alloc );
              src_p += src_size;
              dst_p += dst_p ? dst_size : 0;
            }
          }
          break;
        }
        case Child::kType_Instance:
        {
          dst_child->m_Node->Convert( dst_child_data, src_child_data, src_child->m_Node, alloc );
          break;
        }
      }
    }
  }
  return status;
}

bool sbNode::IsTerminal( const char* data, const sbScalarValue& terminator_value, const char* terminator_name ) const
{
  const Child* child = FindChild( terminator_name );
  assert( child );
  assert( child->m_Type == Child::kType_Scalar );

  sbScalarValue value = child->m_Scalar->ReadValue( data + child->m_Offset );
  return value == terminator_value;
}

int sbNode::GetPointerCount( const Child* child, const char* node_data ) const
{
  int pointer_count = 1;
  if( child->m_CountName )
  {
    const Child* count_child = FindChild( child->m_CountName );
    const char* count_child_data = node_data + count_child->m_Offset;
    pointer_count = ( int )count_child->m_Scalar->ReadValue( count_child_data ).AsInt();
  }
  return pointer_count;
}

int sbNode::GetStringCount( const Child* child, const char* child_data ) const
{
  int string_count = 0;
  const char* p = *( const char** )( child_data );
  bool terminated = false;
  while( !terminated )
  {
    string_count += 1;
    terminated = child->m_Node->IsTerminal( p, child->m_Terminator, child->m_TerminatorName );
    p += child->m_Node->GetSize();
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
    Child* child = m_Children + i;
    size_t child_size = 0;
    size_t child_alignment = 0;
    switch( child->m_Type )
    {
      case Child::kType_Scalar:
      {
        child_size = child->m_Scalar->GetSize();
        child_alignment = child->m_Scalar->GetAlignment();
        break;
      }
      case Child::kType_Pointer:
      case Child::kType_String:
      {
        sbNode* node = schema->FindNode( child->m_LinkName );
        if( !node )
        {
          status = sbStatus_ErrorNodeNotFound;
        }
        if( status == sbStatus_Ok )
        {
          child->m_Node = node;
          child_size = sizeof( void* );
          child_alignment = __alignof( void* );
        }
        break;
      }
      case Child::kType_Instance:
      {
        sbNode* node = schema->FindNode( child->m_LinkName );
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
          child_size = child->m_Node->GetSize();
          child_alignment = child->m_Node->GetAlignment();
        }
        break;
      }
      default:
        assert( 0 );
        break;
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

void sbNode::AddScalar( const char* name, int count, sbScalarType scalar_type )
{
  const sbScalar* scalar = FindScalar( scalar_type );
  assert( scalar );
  Child* child = m_Children + m_ChildCount++;
  
  child->m_Name   = name;
  child->m_LinkName = NULL;
  child->m_Type   = Child::kType_Scalar;
  child->m_Offset = 0;          // To be determined in second pass
  child->m_Count  = count;
  child->m_Scalar = scalar;
  child->m_Node   = NULL;
}

void sbNode::AddInstance( const char* name, int count, const char* link_name )
{
  Child* child = m_Children + m_ChildCount++;
  
  child->m_Name   = name;
  child->m_LinkName = link_name;
  child->m_Type   = Child::kType_Instance;
  child->m_Offset = 0;          // To be determined in second pass
  child->m_Count  = count;
  child->m_Scalar = NULL;
  child->m_Node   = NULL;
}

void sbNode::AddPointer( const char* name, int count, const char* link_name, const char* count_name )
{
  Child* child = m_Children + m_ChildCount++;
  
  child->m_Name   = name;
  child->m_LinkName = link_name;
  child->m_CountName = count_name;
  child->m_Type   = Child::kType_Pointer;
  child->m_Offset = 0;          // To be determined in second pass
  child->m_Count  = count;
  child->m_Scalar = NULL;
  child->m_Node   = NULL;
}

void sbNode::AddString( const char* name, int count, const char* link_name, const sbScalarValue& terminator, const char* terminator_name )
{
  Child* child = m_Children + m_ChildCount++;
  
  child->m_Name   = name;
  child->m_LinkName = link_name;
  child->m_Type   = Child::kType_String;
  child->m_Offset = 0;          // To be determined in second pass
  child->m_Count  = count;
  child->m_Scalar = NULL;
  child->m_Node   = NULL;
  child->m_Terminator = terminator;
  child->m_TerminatorName = terminator_name;
}

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

const sbScalar* FindScalar( sbScalarType scalar_type )
{
  switch( scalar_type )
  {
    case kScalar_U8:  return &s_ScalarU8;
    case kScalar_I8:  return &s_ScalarI8;
    case kScalar_U16: return &s_ScalarU16;
    case kScalar_I16: return &s_ScalarI16;
    case kScalar_U32: return &s_ScalarU32;
    case kScalar_I32: return &s_ScalarI32;
    case kScalar_U64: return &s_ScalarU64;
    case kScalar_I64: return &s_ScalarI64;
    case kScalar_F32: return &s_ScalarF32;
    case kScalar_F64: return &s_ScalarF64;
    default:          return NULL;
  }
}
