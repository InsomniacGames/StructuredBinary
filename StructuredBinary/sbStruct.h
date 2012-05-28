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
#include "sbScalarValue.h"
#include "sbFnv.h"
#include "sbScalar.h"
#include "sbByteWriter.h"
#include "sbByteReader.h"
#include "sbField.h"

enum sbFieldType
{
  sbFieldType_End = 0,
  sbFieldType_I8,
  sbFieldType_U8,
  sbFieldType_I16,
  sbFieldType_U16,
  sbFieldType_I32,
  sbFieldType_U32,
  sbFieldType_I64,
  sbFieldType_U64,
  sbFieldType_F32,
  sbFieldType_F64,
  sbFieldType_Struct,
  sbFieldType_Pointer,
  sbFieldType_Count
};

class sbStruct : public sbField
{
  static const int s_InitialEntryMax = 50;

public:
  sbStruct()
  : m_Size( 0 )
  , m_Align( 1 )
  , m_EntryMax( s_InitialEntryMax )
  , m_EntryCount( 0 )
  , m_Entries( new Entry[ s_InitialEntryMax ] )
  {}

  virtual ~sbStruct();
  virtual const sbStruct* AsStruct() const { return this; }

  void Convert( char* write_data, const char* read_data, const sbStruct* read_struct ) const;

  int GetSize() const;
  int GetAlign() const;

  void WriteSchema( sbByteWriter* writer ) const;
  static const sbStruct* NewFromSchema( sbByteReader* reader );

  void AddScalar( uint32_t name, sbFieldType field_type, int count = 1 );
  void AddStruct( uint32_t name, const sbStruct* str, int count = 1 )
  {
    AddStruct( name, sbFieldType_Struct, count, str );
  }

private:

  void AddField( uint32_t name, sbFieldType field_type, int count, const sbField* field, int offset );
  void AddScalar( uint32_t name, sbFieldType field_type, int count, const sbScalar* scalar );
  void AddStruct( uint32_t name, sbFieldType field_type, int count, const sbStruct* str );

  struct Entry
  {
    sbFieldType     m_Type;
    uint32_t        m_Name;
    int             m_Offset;
    int             m_Count;
    const sbField*  m_Field;
  };
  
  int           m_Size;    // Stored UNALIGNED!!! GetSize() will returned this value aligned.
  int           m_Align;

  int           m_EntryMax;
  int           m_EntryCount;
  Entry*        m_Entries;

  const Entry* FindEntry( uint32_t name ) const;
};

#endif
