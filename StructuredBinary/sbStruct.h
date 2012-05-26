//
//  sbStruct.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbStruct_h
#define StructuredBinary_sbStruct_h

// Libraries
#include <cstring>
#include <string.h>
#include <stdint.h>
// Project
#include "sbNumber.h"
#include "sbFnv.h"
#include "sbField.h"
#include "sbByteWriter.h"
#include "sbByteReader.h"

class sbStruct : public sbField
{
public:
  sbStruct( int field_max_count )
  : m_EntryMax( field_max_count )
  , m_EntryCount( 0 )
  , m_Entry( new Entry[ field_max_count ] )
  {}

  virtual ~sbStruct();

  void AddSubStructTemp( uint32_t name, const sbStruct* agg ) { AddField( name, agg ); }
  
  int GetFieldCount() const { return m_EntryCount; }

  sbNumber Read( const char* data, uint32_t name ) const
  {
    ReadCursor rc = Find( data, name );
    if( rc.m_Data )
    {
      return rc.m_Field->ReadNumber( rc.m_Data );
    }
    return sbNumber::Null();
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
  
  void WriteFormat( sbByteWriter* writer ) const;
  static const sbField* BuildAgg( sbByteReader* reader );

  void AddField( uint32_t name, FieldType field_type );

private:

  void AddField( uint32_t name, const sbField* field );

  struct Entry
  {
    Entry()
    : m_Name( 0 )
    {}
    uint32_t      m_Name;
    int           m_Offset;
    const sbField*  m_Field;
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
