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
#include "sbPath.h"

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

void sbNode::Convert( char* dst_data, const char* src_data, const sbNode* src_node ) const
{
}

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


static sbScalarU8 s_ScalarU8;
static sbScalarI8  s_ScalarI8;
static sbScalarU16 s_ScalarU16;
static sbScalarI16 s_ScalarI16;
static sbScalarU32 s_ScalarU32;
static sbScalarI32 s_ScalarI32;
static sbScalarU64 s_ScalarU64;
static sbScalarI64 s_ScalarI64;
static sbScalarF32 s_ScalarF32;
static sbScalarF64 s_ScalarF64;

const sbScalar* FindScalar( sbScalarType scalar_type )
{
  switch( scalar_type )
  {
    case kScalar_U8:   return &s_ScalarU8;
    case kScalar_I8:   return &s_ScalarI8;
    case kScalar_U16:  return &s_ScalarU16;
    case kScalar_I16:  return &s_ScalarI16;
    case kScalar_U32:  return &s_ScalarU32;
    case kScalar_I32:  return &s_ScalarI32;
    case kScalar_U64:  return &s_ScalarU64;
    case kScalar_I64:  return &s_ScalarI64;
    case kScalar_F32:  return &s_ScalarF32;
    case kScalar_F64:  return &s_ScalarF64;
    default:      return NULL;
  }
}

static void PrintPathName( const sbPath* path )
{
  if( path->m_Parent )
  {
    PrintPathName( path->m_Parent );
  }
  printf( "%s->", path->m_Name );
}

static void PrintPath( const sbPath* path, const char* description, size_t offset, int64_t value )
{
  PrintPathName( path );
  printf( "%s offset:%d value:%lld\n", description, (int)offset, value );
}

void sbNode::PrintNode( const char* data, const sbPath* parent ) const
{
  sbPath path( parent );
  
  for( int i = 0; i < m_ChildCount; ++i )
  {
    const Child* child = m_Children + i;
    const char* child_data = data + child->m_Offset;
    path.m_Name = child->m_Name;
    switch( child->m_Type )
    {
      case Child::kType_Scalar:
      {
        sbScalarValue value = child->m_Scalar->ReadValue( child_data );
        PrintPath( &path, child->m_Scalar->GetDescription(), child->m_Offset, value.AsInt() );
        break;
      }
      case Child::kType_Pointer:
      {
        const char* p = *( const char** )( child_data );
        int pointer_count = 1;
        if( child->m_CountName )
        {
          const Child* count_child = FindChild( child->m_CountName );
          const char* count_child_data = data + count_child->m_Offset;
          pointer_count = ( int )count_child->m_Scalar->ReadValue( count_child_data ).AsInt();
        }
        for( int j = 0; j < pointer_count; ++j )
        {
          child->m_Node->PrintNode( p, &path );
          p += child->m_Node->GetSize();
        }
        break;
      }
      case Child::kType_Instance:
      {
        child->m_Node->PrintNode( child_data, &path );
        break;
      }
    }
  }
}

static size_t FixAlignment( size_t value, size_t alignment )
{
  return value + ( -( value ) & ( alignment - 1 ) );
}

void sbNode::FixUp( sbSchema* schema )
{
  if( m_State == kState_Ready ) return;
  assert( m_State == kState_Defined );

  size_t offset = 0;
  size_t alignment = 1;

  for( int i = 0; i < m_ChildCount; ++i )
  {
    Child* child = m_Children + i;
    switch( child->m_Type )
    {
      case Child::kType_Scalar:
      {
        size_t child_size = child->m_Scalar->GetSize();
        size_t child_alignment = child->m_Scalar->GetAlignment();
        offset = FixAlignment( offset, child_alignment );
        child->m_Offset = offset;
        offset += child_size;
        alignment = child_alignment > alignment ? child_alignment : alignment;
        break;
      }
      case Child::kType_Pointer:
      {
        child->m_Node = schema->FindNode( child->m_LinkName );

        size_t child_size = sizeof( void* );
        size_t child_alignment = __alignof( void* );
        offset = FixAlignment( offset, child_alignment );
        child->m_Offset = offset;
        offset += child_size;
        alignment = child_alignment > alignment ? child_alignment : alignment;
        break;
      }
      case Child::kType_Instance:
      {
        child->m_Node = schema->FindNode( child->m_LinkName );
        child->m_Node->FixUp( schema );

        size_t child_size = child->m_Node->GetSize();
        size_t child_alignment = child->m_Node->GetAlignment();
        offset = FixAlignment( offset, child_alignment );
        child->m_Offset = offset;
        offset += child_size;
        alignment = child_alignment > alignment ? child_alignment : alignment;
        break;
      }
      default:
        assert( 0 );
        break;
    }
  }

  m_Size = FixAlignment( offset, alignment );
  m_Alignment = alignment;
}


