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

enum sbFieldType
{
  kField_End = 0,
  kField_I8,
  kField_U8,
  kField_I16,
  kField_U16,
  kField_I32,
  kField_U32,
  kField_I64,
  kField_U64,
  kField_F32,
  kField_F64,
  kField_Struct,
  kField_Count
};

class sbStruct : public sbField
{
public:
  sbStruct( int field_max_count )
  : m_ElementSize( 0 )
  , m_ElementAlign( 1 )
  , m_EntryMax( field_max_count )
  , m_EntryCount( 0 )
  , m_Entry( new Entry[ field_max_count ] )
  {}

  virtual ~sbStruct();

  void AddSubStructTemp( uint32_t name, const sbStruct* agg, int count = 1 ) { AddField( name, kField_Struct, count, agg ); }

  int GetFieldCount() const { return m_EntryCount; }

  sbNumber Read( const char* data, uint32_t name ) const;
  
  virtual void Convert( char* write_data, const char* read_data, const sbField* read_field ) const;

  virtual int GetElementSize() const;
  virtual int GetElementAlign() const;

  bool IsValid() const { return m_EntryCount <= m_EntryMax; }
  
  sbFieldType GetFieldType( int index ) const;
  const sbField* GetField( int index ) const;
  uint32_t GetFieldName( int index ) const;
  
  virtual void WriteSchema( sbByteWriter* writer ) const;
  static const sbField* ReadSchema( sbByteReader* reader );

  void AddField( uint32_t name, sbFieldType field_type, int count = 1 );

private:

  void AddField( uint32_t name, sbFieldType field_type, int count, const sbField* field );

  struct Entry
  {
    sbFieldType     m_Type;
    uint32_t        m_Name;
    int             m_Offset;
    int             m_Count;
    const sbField*  m_Field;
  };

  int     m_ElementSize;    // Stored UNALIGNED!!! GetElementSize() will returned this value aligned.
  int     m_ElementAlign;
  int     m_EntryMax;
  int     m_EntryCount;
  Entry*  m_Entry;

  const Entry* FindEntry( uint32_t name ) const;
};

#endif
