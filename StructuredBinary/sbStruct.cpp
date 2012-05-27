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
  for( int i = 0; i < m_ScalarCount; ++i )
  {
    const ScalarEntry* write_entry = m_Scalars + i;
    uint32_t name = write_entry->m_Name;
    const ScalarEntry* read_entry = read_struct->FindScalarEntry( name );
    // What if not found??? Default!
    char* write_element = write_data + write_entry->m_Offset;
    const char* read_element = read_data + read_entry->m_Offset;

    int write_size = write_entry->m_Scalar->GetElementSize();
    int read_size = read_entry->m_Scalar->GetElementSize();

    int count = m_Scalars[ i ].m_Count;
    for( int j = 0; j < count; ++j )
    {
      write_entry->m_Scalar->Convert( write_element, read_element, read_entry->m_Scalar );
      write_element += write_size;
      read_element += read_size;
    }
  }

  // Convert structs
  for( int i = 0; i < m_StructCount; ++i )
  {
    const StructEntry* write_entry = m_Structs + i;
    uint32_t name = write_entry->m_Name;
    const StructEntry* read_entry = read_struct->FindStructEntry( name );
    // What if not found??? Default!
    char* write_element = write_data + write_entry->m_Offset;
    const char* read_element = read_data + read_entry->m_Offset;
    
    int write_size = write_entry->m_Struct->GetElementSize();
    int read_size = read_entry->m_Struct->GetElementSize();
    
    int count = write_entry->m_Count;
    for( int j = 0; j < count; ++j )
    {
      write_entry->m_Struct->Convert( write_element, read_element, read_entry->m_Struct );
      write_element += write_size;
      read_element += read_size;
    }
  }
}

const sbStruct::StructEntry* sbStruct::FindStructEntry( uint32_t name ) const
{
  const StructEntry* entry = m_Structs;
  for( int i = 0; i < m_StructCount; ++i )
  {
    if( entry->m_Name == name )
    {
      return entry;
    }    
    entry += 1;
  }
  return NULL;
}

const sbStruct::ScalarEntry* sbStruct::FindScalarEntry( uint32_t name ) const
{
  const ScalarEntry* entry = m_Scalars;
  for( int i = 0; i < m_ScalarCount; ++i )
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

int sbStruct::GetElementSize() const
{
  return FixAlign( m_ElementSize, m_ElementAlign );
}

int sbStruct::GetElementAlign() const
{
  return m_ElementAlign;
}

void sbStruct::AddScalar( uint32_t name, sbFieldType field_type, int count, const sbScalar* scalar )
{
  int index = m_ScalarCount++;
  if( index < m_ScalarMax )
  {
    int scalar_size = scalar->GetElementSize();
    int scalar_align = scalar->GetElementAlign();
    int scalar_offset = FixAlign( m_ElementSize, scalar_align );
    
    ScalarEntry* entry = m_Scalars + index;

    entry->m_Name   = name;
    entry->m_Offset = scalar_offset;
    entry->m_Type   = field_type;
    entry->m_Count  = count;
    entry->m_Scalar  = scalar;
    
    m_ElementSize = scalar_offset + scalar_size * count;
    m_ElementAlign = scalar_align > m_ElementAlign ? scalar_align : m_ElementAlign;
  }
}

void sbStruct::AddStruct( uint32_t name, sbFieldType field_type, int count, const sbStruct* str )
{
  int index = m_StructCount++;
  if( index < m_StructMax )
  {
    int scalar_size = str->GetElementSize();
    int scalar_align = str->GetElementAlign();
    int scalar_offset = FixAlign( m_ElementSize, scalar_align );

    StructEntry* entry = m_Structs + index;

    entry->m_Name   = name;
    entry->m_Offset = scalar_offset;
    entry->m_Type   = field_type;
    entry->m_Count  = count;
    entry->m_Struct = str;

    m_ElementSize = scalar_offset + scalar_size * count;
    m_ElementAlign = scalar_align > m_ElementAlign ? scalar_align : m_ElementAlign;
  }
}


sbStruct::~sbStruct()
{
  delete[] m_Scalars;

  for( int i = 0; i < m_StructCount; ++i )
  {
    delete m_Structs[ i ].m_Struct;
  }
  delete[] m_Structs;
}

static const sbScalar* NewScalar( sbFieldType field_type )
{
  switch( field_type )
  {
    case sbFieldType_ScalarF64:     return &s_ScalarF64;
    case sbFieldType_ScalarF32:     return &s_ScalarF32;
    case sbFieldType_ScalarI64:     return &s_ScalarI64;
    case sbFieldType_ScalarU64:     return &s_ScalarU64;
    case sbFieldType_ScalarI32:     return &s_ScalarI32;
    case sbFieldType_ScalarU32:     return &s_ScalarU32;
    case sbFieldType_ScalarI16:     return &s_ScalarI16;
    case sbFieldType_ScalarU16:     return &s_ScalarU16;
    case sbFieldType_ScalarI8:      return &s_ScalarI8;
    case sbFieldType_ScalarU8:      return &s_ScalarU8;
    default:
      assert( false );
      return NULL;
  }
}

const sbStruct* sbStruct::ReadSchema( sbByteReader* reader )
{
  sbStruct* a = new sbStruct( 100, 100 );
  while( reader->GetRemain() > 0 )
  {
    uint32_t name = reader->Read32();
    sbFieldType field_type = ( sbFieldType )reader->Read8();
    const sbScalar* f = NULL;

    switch( field_type )
    {
      case sbFieldType_Struct:
        f = new sbScalarF64();
        break;
      default:
        f = NewScalar( field_type );
        break;
    }

    a->AddScalar( name, field_type, 1, f );
  }
  return a;
}

void sbStruct::WriteSchema( sbByteWriter* writer ) const
{
  for( int i = 0; i < m_ScalarCount; ++i )
  {
    ScalarEntry* entry = m_Scalars + i;

    writer->Write32( entry->m_Name );
    writer->Write8( ( uint8_t )entry->m_Type );
  }
}

void sbStruct::AddScalar( uint32_t name, sbFieldType field_type, int count )
{
  const sbScalar* f = NewScalar( field_type );
  AddScalar( name, field_type, count, f );
}
