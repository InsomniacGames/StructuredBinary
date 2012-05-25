//
//  Aggregate.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_Aggregate_h
#define StructuredBinary_Aggregate_h

// Libraries
#include <cstring>
#include <string.h>
#include <stdint.h>
// Project
#include "Number.h"
#include "Fnv.h"
#include "Field.h"

class Aggregate : public Field
{
public:
  Aggregate( int field_max_count )
  : m_EntryMax( field_max_count )
  , m_EntryCount( 0 )
  , m_Entry( new Entry[ field_max_count ] )
  {}

  ~Aggregate()
  {
    delete[] m_Entry;
  }

  void AddFloat64 ( uint32_t name )  { AddField( name, &s_Float64 ); }
  void AddFloat32 ( uint32_t name )  { AddField( name, &s_Float32 ); }
  void AddInt64   ( uint32_t name )  { AddField( name, &s_Int64   ); }
  void AddUInt64  ( uint32_t name )  { AddField( name, &s_UInt64  ); }
  void AddInt32   ( uint32_t name )  { AddField( name, &s_Int32   ); }
  void AddUInt32  ( uint32_t name )  { AddField( name, &s_UInt32  ); }
  void AddInt16   ( uint32_t name )  { AddField( name, &s_Int16   ); }
  void AddUInt16  ( uint32_t name )  { AddField( name, &s_UInt16  ); }
  void AddInt8    ( uint32_t name )  { AddField( name, &s_Int8    ); }
  void AddUInt8   ( uint32_t name )  { AddField( name, &s_UInt8   ); }
  void AddSubStruct( uint32_t name, const Aggregate* agg ) { AddField( name, agg ); }
  
  int GetFieldCount() const { return m_EntryCount; }

  Number Read( const char* data, uint32_t name ) const
  {
    ReadCursor rc = Find( data, name );
    if( rc.m_Data )
    {
      return rc.m_Field->ReadNumber( rc.m_Data );
    }
    return Number::Null();
  }

  virtual ReadCursor Find( const char* data, uint32_t name ) const
  {
    for( int i = 0; i < m_EntryCount; ++i )
    {
      if( m_Entry[ i ].m_Name == name )
        return ReadCursor( data + m_Entry[ i ].m_Offset, m_Entry[ i ].m_Name, m_Entry[ i ].m_Field );
    }
    return ReadCursor( NULL, 0, NULL );
  }
  
  virtual WriteCursor Find( char* data, uint32_t name ) const
  {
    for( int i = 0; i < m_EntryCount; ++i )
    {
      if( m_Entry[ i ].m_Name == name )
        return WriteCursor( data + m_Entry[ i ].m_Offset, m_Entry[ i ].m_Name, m_Entry[ i ].m_Field );
    }
    return WriteCursor( NULL, 0, NULL );
  }
  
  virtual void Convert( char* dst_data, const ReadCursor& rc ) const;

  virtual void FixSizeAndStride();
  virtual int GetElementSize() const { return m_ElementSize; }
  virtual int GetElementStride() const { return m_ElementStride; }
  virtual int GetElementAlign() const { return m_ElementAlign; }
  virtual FieldType GetType() const { return kField_Agg; }

  bool IsValid() const { return m_EntryCount <= m_EntryMax; }
  
  FieldType GetFieldType( int index ) const;
  uint32_t GetFieldName( int index ) const;

private:

  void AddField( uint32_t name, const Field* field )
  {
    int index = m_EntryCount++;
    if( index < m_EntryMax )
    {
      m_Entry[ index ].m_Name = name;
      m_Entry[ index ].m_Offset = -1; // This will be set later, by a call to FixSizeAndStride
      m_Entry[ index ].m_Field = field;
    }
  }

  struct Entry
  {
    Entry()
    : m_Name( 0 )
    {}
    uint32_t      m_Name;
    int           m_Offset;
    const Field*  m_Field;
  };

  int     m_ElementSize;
  int     m_ElementStride;
  int     m_ElementAlign;
  int     m_EntryMax;
  int     m_EntryCount;
  Entry*  m_Entry;
  
  static FieldFloat32  s_Float32;
  static FieldFloat64  s_Float64;
  static FieldInt64    s_Int64;
  static FieldInt32    s_Int32;
  static FieldInt16    s_Int16;
  static FieldInt8     s_Int8;
  static FieldUInt64   s_UInt64;
  static FieldUInt32   s_UInt32;
  static FieldUInt16   s_UInt16;
  static FieldUInt8    s_UInt8;
};

class DataDesc;

void Convert( char* dst_data, const DataDesc* dst_desc, const char* src_data, const DataDesc* src_desc );

#endif
