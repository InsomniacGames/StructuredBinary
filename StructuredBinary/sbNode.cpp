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

bool sbNode::IsTerminal( const char* data, const sbScalarValue& terminator_value, const char* terminator_name ) const
{
  const Child* child = FindChild( terminator_name );
  assert( child );
  assert( child->m_Type == Child::kType_Scalar );

  sbScalarValue value = child->m_Scalar->ReadValue( data );
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

void sbNode::PrintNode( const char* node_data, const sbPath* parent ) const
{
  sbPath path( parent );
  
  for( int i = 0; i < m_ChildCount; ++i )
  {
    const Child* child = m_Children + i;
    path.m_Name = child->m_Name;
    for( int index = 0; index < child->m_Count; ++index )
    {
      const char* child_data = node_data + child->m_Offset + index * child->m_ElementSize;

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
          int pointer_count = GetPointerCount( child, node_data );

          const char* p = *( const char** )( child_data );
          for( int j = 0; j < pointer_count; ++j )
          {
            child->m_Node->PrintNode( p, &path );
            p += child->m_Node->GetSize();
          }
          break;
        }
        case Child::kType_String:
        {
          int string_count = GetStringCount( child, child_data );

          const char* p = *( const char** )( child_data );
          for( int j = 0; j < string_count; ++j )
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
        child->m_Node = schema->FindNode( child->m_LinkName );
        
        child_size = sizeof( void* );
        child_alignment = __alignof( void* );
        break;
      }
      case Child::kType_Instance:
      {
        child->m_Node = schema->FindNode( child->m_LinkName );
        child->m_Node->FixUp( schema );
        child_size = child->m_Node->GetSize();
        child_alignment = child->m_Node->GetAlignment();
        break;
      }
      default:
        assert( 0 );
        break;
    }

    offset = FixAlignment( offset, child_alignment );
    child->m_Offset = offset;
    offset += child_size * child->m_Count;
    alignment = child_alignment > alignment ? child_alignment : alignment;
    child->m_ElementSize = child_size;
  }

  m_Size = FixAlignment( offset, alignment );
  m_Alignment = alignment;
}


