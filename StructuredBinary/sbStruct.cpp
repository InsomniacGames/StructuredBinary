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
// Project
#include "sbNumber.h"
#include "sbFnv.h"
#include "sbField.h"

void sbStruct::Convert( char* dst_data, const ReadCursor& rc ) const
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    uint32_t name = m_Entry[ i ].m_Name;
    m_Entry[ i ].m_Field->Convert( dst_data + m_Entry[ i ].m_Offset, rc.m_Field->Find( rc.m_Data, name ) );
  }
}

ReadCursor sbStruct::Find( const char* data, uint32_t name ) const
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

WriteCursor sbStruct::Find( char* data, uint32_t name ) const
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    if( m_Entry[ i ].m_Name == name )
      return WriteCursor( data + m_Entry[ i ].m_Offset, m_Entry[ i ].m_Name, m_Entry[ i ].m_Field );
  }
  return WriteCursor( NULL, 0, NULL );
}

void sbStruct::FixSizeAndStride()
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

FieldType sbStruct::GetFieldType( int index ) const
{
  assert( index < m_EntryCount );
  return m_Entry[ index ].m_Field->GetType();
}

uint32_t sbStruct::GetFieldName( int index ) const
{
  assert( index < m_EntryCount );
  return m_Entry[ index ].m_Name;
}

void sbStruct::WriteFormat( sbByteWriter* writer ) const
{
  for( int i = 0; i < GetFieldCount(); ++i )
  {
    uint32_t name = GetFieldName( i ); 
    uint8_t field_type = GetFieldType( i );
    writer->Write32( name );
    writer->Write8( field_type );
  }
}

void sbStruct::AddField( uint32_t name, const sbField* field )
{
  int index = m_EntryCount++;
  if( index < m_EntryMax )
  {
    m_Entry[ index ].m_Name = name;
    m_Entry[ index ].m_Offset = -1; // This will be set later, by a call to FixSizeAndStride
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

const sbField* sbStruct::BuildAgg( sbByteReader* reader )
{
  sbStruct* a = new sbStruct( 100 );
  while( reader->GetRemain() > 0 )
  {
    uint32_t name = reader->Read32();
    FieldType field_type = ( FieldType )reader->Read8();
    const sbField* f = GetInfo( field_type ).build_field( reader );  
    a->AddField( name, f );
  }
  a->FixSizeAndStride();
  return a;
}

void sbStruct::AddField( uint32_t name, FieldType field_type )
{
  const sbField* f = GetInfo( field_type ).build_field( NULL );   // ???
  AddField( name, f );
}


