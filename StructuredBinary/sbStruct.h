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

enum sbFieldType
{
  sbFieldType_End = 0,
  sbFieldType_ScalarI8,
  sbFieldType_ScalarU8,
  sbFieldType_ScalarI16,
  sbFieldType_ScalarU16,
  sbFieldType_ScalarI32,
  sbFieldType_ScalarU32,
  sbFieldType_ScalarI64,
  sbFieldType_ScalarU64,
  sbFieldType_ScalarF32,
  sbFieldType_ScalarF64,
  sbFieldType_Struct,
  sbFieldType_Count
};

class sbStruct
{
public:
  sbStruct( int scalar_max, int struct_max )
  : m_ElementSize( 0 )
  , m_ElementAlign( 1 )
  , m_ScalarMax( scalar_max )
  , m_StructMax( struct_max )
  , m_ScalarCount( 0 )
  , m_StructCount( 0 )
  , m_Scalars( new ScalarEntry[ scalar_max ] )
  , m_Structs( new StructEntry[ struct_max ] )
  {}

  ~sbStruct();

  int GetScalarCount() const { return m_ScalarCount; }
  int GetStructCount() const { return m_StructCount; }

  void Convert( char* write_data, const char* read_data, const sbStruct* read_struct ) const;

  int GetElementSize() const;
  int GetElementAlign() const;
/*
  sbFieldType GetScalarType( int index ) const;
  const sbScalar* GetScalar( int index ) const;
  uint32_t GetScalarName( int index ) const;
*/
  virtual void WriteSchema( sbByteWriter* writer ) const;
  static const sbStruct* ReadSchema( sbByteReader* reader );

  void AddScalar( uint32_t name, sbFieldType field_type, int count = 1 );
  void AddStruct( uint32_t name, const sbStruct* str, int count = 1 )
  {
    AddStruct( name, sbFieldType_Struct, count, str );
  }

private:

  void AddScalar( uint32_t name, sbFieldType field_type, int count, const sbScalar* scalar );
  void AddStruct( uint32_t name, sbFieldType field_type, int count, const sbStruct* str );

  struct ScalarEntry
  {
    sbFieldType      m_Type;
    uint32_t          m_Name;
    int               m_Offset;
    int               m_Count;
    const sbScalar*    m_Scalar;
  };
  
  struct StructEntry
  {
    sbFieldType      m_Type;
    uint32_t          m_Name;
    int               m_Offset;
    int               m_Count;
    const sbStruct*   m_Struct;
  };
  
  int           m_ElementSize;    // Stored UNALIGNED!!! GetElementSize() will returned this value aligned.
  int           m_ElementAlign;

  int           m_ScalarMax;
  int           m_StructMax;

  int           m_ScalarCount;
  int           m_StructCount;

  ScalarEntry*   m_Scalars;
  StructEntry*  m_Structs;

  const ScalarEntry* FindScalarEntry( uint32_t name ) const;
  const StructEntry* FindStructEntry( uint32_t name ) const;
};

#endif
