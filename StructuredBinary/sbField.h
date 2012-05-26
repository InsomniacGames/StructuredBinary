//
//  sbField.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbField_h
#define StructuredBinary_sbField_h

// Libraries
#include <cstring>
#include <stdint.h>

// Project
#include "sbNumber.h"
#include "sbFnv.h"
#include "sbReadCursor.h"
#include "sbWriteCursor.h"

class sbField;
class sbByteReader;
class sbByteWriter;

enum sbFieldType
{
  kField_Unknown = 0,
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
  kField_Agg,
  kField_Count
};

typedef const sbField* ( sbReadField )( sbByteReader* reader );

struct sbFieldTypeInfo
{
  sbFieldType   field_type;
  const char*   description;
  sbReadField*  read_field;
};

const sbFieldTypeInfo& GetInfo( sbFieldType field_type );

class sbField
{
public:
  virtual ~sbField() {}
  virtual sbNumber ReadNumber( const char* data ) const { return sbNumber::Null(); }
  virtual void WriteNumber( char* data, const sbNumber& number ) const {}

  virtual void Convert( char* dst_data, const ReadCursor& rc ) const
  {
    sbNumber n = rc.m_Field->ReadNumber( rc.m_Data );
    WriteNumber( dst_data, n );
  }

  virtual ReadCursor Find( const char* data, uint32_t name ) const
  {
    return ReadCursor( NULL, 0, NULL );
  }

  virtual WriteCursor Find( char* data, uint32_t name ) const
  {
    return WriteCursor( NULL, 0, NULL );
  }

  virtual void FixSizeAndStride() {}
  virtual int GetElementSize() const = 0;
  virtual int GetElementStride() const { return GetElementSize(); }
  virtual int GetElementAlign() const = 0;
  virtual sbFieldType GetType() const = 0;

  virtual void WriteSchema( sbByteWriter* writer ) const = 0;

private:
};

#endif
