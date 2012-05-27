//
//  sbStruct.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "sbStruct.h"
// Libraries
#include <cstring>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
// Project
#include "sbNumber.h"
#include "sbFnv.h"
#include "sbField.h"

void sbStruct::Convert( char* write_data, const char* read_data, const sbField* read_field ) const
{
  const sbStruct* read_struct = ( const sbStruct* )read_field;  // !!! UGH
  const sbStruct::Entry* write_entry = m_Entry;
  for( int i = 0; i < m_EntryCount; ++i )
  {
    uint32_t name = write_entry->m_Name;
    const Entry* read_entry = read_struct->FindEntry( name );
    
    char* write_element = write_data + m_Entry[ i ].m_Offset;
    const char* read_element = read_data + read_entry->m_Offset;

    int write_stride = write_entry->m_Field->GetElementStride();
    int read_stride = read_entry->m_Field->GetElementStride();

    int count = write_entry->m_Count;
    for( int j = 0; j < count; ++j )
    {
      write_entry->m_Field->Convert( write_element, read_element, read_entry->m_Field );
      write_element += write_stride;
      read_element += read_stride;
    }
    
    write_entry += 1;
  }
}

const sbStruct::Entry* sbStruct::FindEntry( uint32_t name ) const
{
  const sbStruct::Entry* entry = m_Entry;
  for( int i = 0; i < m_EntryCount; ++i )
  {
    if( entry->m_Name == name )
    {
      return entry;
    }    
    entry += 1;
  }
  return NULL;
}

void sbStruct::FixSizeAndStride()
{
  int offset = 0;
  int max_align = 1;
  for( int i = 0; i < m_EntryCount; ++i )
  {
    int field_stride = m_Entry[ i ].m_Field->GetElementStride();
    int field_align = m_Entry[ i ].m_Field->GetElementAlign();
    int count = m_Entry[ i ].m_Count;
    if( max_align < field_align )
    {
      max_align = field_align;
    }
    offset += ( -offset ) & ( field_align - 1 );
    m_Entry[ i ].m_Offset = offset;
    offset += field_stride * count;
  }
  m_ElementSize = offset;
  m_ElementStride = offset;
  m_ElementAlign = max_align;
}

sbFieldType sbStruct::GetFieldType( int index ) const
{
  assert( index < m_EntryCount );
  return m_Entry[ index ].m_Type;
}

const sbField* sbStruct::GetField( int index ) const
{
  assert( index < m_EntryCount );
  return m_Entry[ index ].m_Field;
}

uint32_t sbStruct::GetFieldName( int index ) const
{
  assert( index < m_EntryCount );
  return m_Entry[ index ].m_Name;
}

void sbStruct::AddField( uint32_t name, sbFieldType field_type, int count, const sbField* field )
{
  int index = m_EntryCount++;
  if( index < m_EntryMax )
  {
    m_Entry[ index ].m_Name = name;
    m_Entry[ index ].m_Offset = -1; // This will be set later, by a call to FixSizeAndStride
    m_Entry[ index ].m_Type = field_type;
    m_Entry[ index ].m_Count = count;
    m_Entry[ index ].m_Field = field;
  }
}


sbStruct::~sbStruct()
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    delete m_Entry[ i ].m_Field;
  }

  delete[] m_Entry;
}

static const sbField* NewScalar( sbFieldType field_type )
{
  switch( field_type )
  {
    case kField_I8:
      return new sbFieldI8();
    case kField_U8:
      return new sbFieldU8();
    case kField_I16:
      return new sbFieldI16();
    case kField_U16:
      return new sbFieldU16();
    case kField_I32:
      return new sbFieldI32();
    case kField_U32:
      return new sbFieldU32();
    case kField_I64:
      return new sbFieldI64();
    case kField_U64:
      return new sbFieldU64();
    case kField_F32:
      return new sbFieldF32();
    case kField_F64:
      return new sbFieldF64();
    case kField_Struct:
      return new sbFieldF64();
    default:
      assert( false );
  }
}

const sbField* sbStruct::ReadSchema( sbByteReader* reader )
{
  sbStruct* a = new sbStruct( 100 );
  while( reader->GetRemain() > 0 )
  {
    uint32_t name = reader->Read32();
    sbFieldType field_type = ( sbFieldType )reader->Read8();
    const sbField* f = NULL;

    switch( field_type )
    {
      case kField_Struct:
        f = new sbFieldF64();
        break;
      default:
        f = NewScalar( field_type );
        break;
    }

    a->AddField( name, field_type, 1, f );
  }
  a->FixSizeAndStride();
  return a;
}

void sbStruct::WriteSchema( sbByteWriter* writer ) const
{
  for( int i = 0; i < GetFieldCount(); ++i )
  {
    writer->Write32( GetFieldName( i ) );
    writer->Write8( GetFieldType( i ) );
  }
}

void sbStruct::AddField( uint32_t name, sbFieldType field_type, int count )
{
  const sbField* f = NewScalar( field_type );
  AddField( name, field_type, count, f );
}

sbNumber sbStruct::Read( const char* read_data, uint32_t name ) const
{
  const Entry* read_entry = FindEntry( name );
  if( read_entry )
  {
    return read_entry->m_Field->ReadNumber( read_data + read_entry->m_Offset );
  }
  return sbNumber::Null();
}


