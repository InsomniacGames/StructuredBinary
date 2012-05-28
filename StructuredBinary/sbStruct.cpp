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
#include "sbScalarValue.h"
#include "sbFnv.h"
#include "sbScalar.h"

sbScalarF64 s_ScalarF64;
sbScalarF32 s_ScalarF32;
sbScalarU64 s_ScalarU64;
sbScalarI64 s_ScalarI64;
sbScalarU32 s_ScalarU32;
sbScalarI32 s_ScalarI32;
sbScalarU16 s_ScalarU16;
sbScalarI16 s_ScalarI16;
sbScalarU8 s_ScalarU8;
sbScalarI8 s_ScalarI8;

void sbStruct::Convert( char* write_data, const char* read_data, const sbStruct* read_struct ) const
{
  // Convert scalars
  for( int i = 0; i < m_EntryCount; ++i )
  {
    const Entry* write_entry = m_Entries + i;
    const sbScalar* write_scalar = write_entry->m_Field->AsScalar();
    if( !write_scalar ) continue;
    uint32_t name = write_entry->m_Name;
    const Entry* read_entry = read_struct->FindEntry( name );
    const sbScalar* read_scalar = read_entry->m_Field->AsScalar();

    // What if not found??? Default!
    char* write_element = write_data + write_entry->m_Offset;
    const char* read_element = read_data + read_entry->m_Offset;

    int write_size = write_scalar->GetSize();
    int read_size = read_scalar->GetSize();

    int count = read_entry->m_Count;
    for( int j = 0; j < count; ++j )
    {
      write_scalar->Convert( write_element, read_element, read_scalar );
      write_element += write_size;
      read_element += read_size;
    }
  }

  // Convert structs
  for( int i = 0; i < m_EntryCount; ++i )
  {
    const Entry* write_entry = m_Entries + i;
    const sbStruct* write_struct = write_entry->m_Field->AsStruct();
    if( !write_struct ) continue;
    uint32_t name = write_entry->m_Name;
    const Entry* read_entry = read_struct->FindEntry( name );
    const sbStruct* read_struct = read_entry->m_Field->AsStruct();

    // What if not found??? Default!
    char* write_element = write_data + write_entry->m_Offset;
    const char* read_element = read_data + read_entry->m_Offset;
    
    int write_size = write_struct->GetSize();
    int read_size = read_struct->GetSize();
    
    int count = write_entry->m_Count;
    for( int j = 0; j < count; ++j )
    {
      write_struct->Convert( write_element, read_element, read_struct );
      write_element += write_size;
      read_element += read_size;
    }
  }
}

const sbStruct::Entry* sbStruct::FindEntry( uint32_t name ) const
{
  const Entry* entry = m_Entries;
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

static int FixAlign( int value, int alignment )
{
  return value + ( ( -value ) & ( alignment - 1 ) );
}

int sbStruct::GetSize() const
{
  return FixAlign( m_Size, m_Align );
}

int sbStruct::GetAlign() const
{
  return m_Align;
}

void sbStruct::AddField( uint32_t name, sbFieldType field_type, int count, const sbField* field, int offset )
{
  int index = m_EntryCount++;
  if( index < m_EntryMax )
  {    
    Entry* entry = m_Entries + index;
    
    entry->m_Name   = name;
    entry->m_Offset = offset;
    entry->m_Type   = field_type;
    entry->m_Count  = count;
    entry->m_Field  = field;
  }
}

void sbStruct::AddScalar( uint32_t name, sbFieldType field_type, int count, const sbScalar* scalar )
{
  int size = scalar->GetSize();
  int align = scalar->GetAlign();
  int offset = FixAlign( m_Size, align );
  
  AddField( name, field_type, count, scalar, offset );
  
  m_Size = offset + size * count;
  m_Align = align > m_Align ? align : m_Align;
}

void sbStruct::AddStruct( uint32_t name, sbFieldType field_type, int count, const sbStruct* str )
{
  int size = str->GetSize();
  int align = str->GetAlign();
  int offset = FixAlign( m_Size, align );
  
  AddField( name, field_type, count, str, offset );
  
  m_Size = offset + size * count;
  m_Align = align > m_Align ? align : m_Align;
}


sbStruct::~sbStruct()
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    const sbStruct* s = m_Entries[ i ].m_Field->AsStruct();
    if( s ) delete s;
  }
  delete[] m_Entries;
}

static const sbScalar* FindScalar( sbFieldType field_type )
{
  switch( field_type )
  {
    case sbFieldType_F64:     return &s_ScalarF64;
    case sbFieldType_F32:     return &s_ScalarF32;
    case sbFieldType_I64:     return &s_ScalarI64;
    case sbFieldType_U64:     return &s_ScalarU64;
    case sbFieldType_I32:     return &s_ScalarI32;
    case sbFieldType_U32:     return &s_ScalarU32;
    case sbFieldType_I16:     return &s_ScalarI16;
    case sbFieldType_U16:     return &s_ScalarU16;
    case sbFieldType_I8:      return &s_ScalarI8;
    case sbFieldType_U8:      return &s_ScalarU8;
    default:
      assert( false );
      return NULL;
  }
}

const sbStruct* sbStruct::NewFromSchema( sbByteReader* reader )
{
  sbStruct* a = new sbStruct;

  a->m_Size = reader->Read32();
  a->m_Align = reader->Read8();

  sbFieldType field_type = ( sbFieldType )reader->Read8();
  while( field_type != sbFieldType_End )
  {
    uint16_t offset = reader->Read16();
    uint32_t name = reader->Read32();

    switch( field_type )
    {
      case sbFieldType_Struct:
      {
        const sbStruct* s = NewFromSchema( reader );
        a->AddField( name, field_type, 1, s, offset );
        break;
      }
      default:
      {
        const sbScalar* f = FindScalar( field_type );
        a->AddField( name, field_type, 1, f, offset );
        break;
      }
    }

    field_type = ( sbFieldType )reader->Read8();
  }
  return a;
}

void sbStruct::WriteSchema( sbByteWriter* writer ) const
{
  writer->Write32( m_Size );
  writer->Write8( m_Align );
  
  for( int i = 0; i < m_EntryCount; ++i )
  {
    Entry* entry = m_Entries + i;
    switch( entry->m_Type )
    {
      case sbFieldType_Struct:
        writer->Write8( ( uint8_t )entry->m_Type );
        writer->Write16( entry->m_Offset );
        writer->Write32( entry->m_Name );
        entry->m_Field->AsStruct()->WriteSchema( writer );
        break;
      default:
        writer->Write8( ( uint8_t )entry->m_Type );
        writer->Write16( entry->m_Offset );
        writer->Write32( entry->m_Name );
        break;
    }
  }
  
  writer->Write8( ( uint8_t )sbFieldType_End );
}

void sbStruct::AddScalar( uint32_t name, sbFieldType field_type, int count )
{
  const sbScalar* f = FindScalar( field_type );
  AddScalar( name, field_type, count, f );
}

