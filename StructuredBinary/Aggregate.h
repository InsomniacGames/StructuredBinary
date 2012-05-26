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
#include "ByteWriter.h"
#include "ByteReader.h"

class Aggregate : public Field
{
public:
  Aggregate( int field_max_count )
  : m_EntryMax( field_max_count )
  , m_EntryCount( 0 )
  , m_Entry( new Entry[ field_max_count ] )
  {}

  virtual ~Aggregate();

  void AddField( uint32_t name, FieldType field_type );
  void AddSubStructTemp( uint32_t name, const Aggregate* agg ) { AddField( name, agg ); }
  
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

  virtual ReadCursor Find( const char* data, uint32_t name ) const;
  virtual WriteCursor Find( char* data, uint32_t name ) const;
  
  virtual void Convert( char* dst_data, const ReadCursor& rc ) const;

  virtual void FixSizeAndStride();
  virtual int GetElementSize() const { return m_ElementSize; }
  virtual int GetElementStride() const { return m_ElementStride; }
  virtual int GetElementAlign() const { return m_ElementAlign; }
  virtual FieldType GetType() const { return kField_Agg; }

  bool IsValid() const { return m_EntryCount <= m_EntryMax; }
  
  FieldType GetFieldType( int index ) const;
  uint32_t GetFieldName( int index ) const;
  
  void WriteFormat( ByteWriter* writer ) const;
  void ReadFormat( ByteReader* reader );

private:

  void AddField( uint32_t name, const Field* field );

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
};

class DataDesc;

void Convert( char* dst_data, const DataDesc* dst_desc, const char* src_data, const DataDesc* src_desc );

#endif
