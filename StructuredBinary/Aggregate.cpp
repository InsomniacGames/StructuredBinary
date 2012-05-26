//
//  Aggregate.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "Aggregate.h"
// Libraries
#include <cstring>
#include <stdint.h>
#include <assert.h>
// Project
#include "Number.h"
#include "Fnv.h"
#include "Field.h"

void Aggregate::Convert( char* dst_data, const ReadCursor& rc ) const
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    uint32_t name = m_Entry[ i ].m_Name;
    m_Entry[ i ].m_Field->Convert( dst_data + m_Entry[ i ].m_Offset, rc.m_Field->Find( rc.m_Data, name ) );
  }
}

ReadCursor Aggregate::Find( const char* data, uint32_t name ) const
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    if( m_Entry[ i ].m_Name == name )
    {
      return ReadCursor( data + m_Entry[ i ].m_Offset, m_Entry[ i ].m_Name, m_Entry[ i ].m_Field );
    }
  }
  return ReadCursor( NULL, 0, NULL );
}

WriteCursor Aggregate::Find( char* data, uint32_t name ) const
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    if( m_Entry[ i ].m_Name == name )
      return WriteCursor( data + m_Entry[ i ].m_Offset, m_Entry[ i ].m_Name, m_Entry[ i ].m_Field );
  }
  return WriteCursor( NULL, 0, NULL );
}

void Aggregate::FixSizeAndStride()
{
  int offset = 0;
  int max_align = 1;
  for( int i = 0; i < m_EntryCount; ++i )
  {
    int field_stride = m_Entry[ i ].m_Field->GetElementStride();
    int field_align = m_Entry[ i ].m_Field->GetElementAlign();
    if( max_align < field_align )
    {
      max_align = field_align;
    }
    offset += ( -offset ) & ( field_align - 1 );
    m_Entry[ i ].m_Offset = offset;
    offset += field_stride;
  }
  m_ElementSize = offset;
  m_ElementStride = offset;
  m_ElementAlign = max_align;
}

FieldType Aggregate::GetFieldType( int index ) const
{
  assert( index < m_EntryCount );
  return m_Entry[ index ].m_Field->GetType();
}

uint32_t Aggregate::GetFieldName( int index ) const
{
  assert( index < m_EntryCount );
  return m_Entry[ index ].m_Name;
}

void Aggregate::WriteFormat( ByteWriter* writer ) const
{
  for( int i = 0; i < GetFieldCount(); ++i )
  {
    uint32_t name = GetFieldName( i ); 
    uint8_t field_type = GetFieldType( i );
    writer->Write32( name );
    writer->Write8( field_type );
  }
}

void Aggregate::ReadFormat( ByteReader* reader )
{
}

void Aggregate::AddField( uint32_t name, const Field* field )
{
  int index = m_EntryCount++;
  if( index < m_EntryMax )
  {
    m_Entry[ index ].m_Name = name;
    m_Entry[ index ].m_Offset = -1; // This will be set later, by a call to FixSizeAndStride
    m_Entry[ index ].m_Field = field;
  }
}

void Aggregate::AddField( uint32_t name, FieldType field_type )
{
  AddField( name, GetInfo( field_type ).build_field() );  
}

Aggregate::~Aggregate()
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    delete m_Entry[ i ].m_Field;
  }

  delete[] m_Entry;
}


