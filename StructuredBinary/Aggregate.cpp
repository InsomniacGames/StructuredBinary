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
// Project
#include "Number.h"
#include "Fnv.h"
#include "Field.h"

FieldFloat32  Aggregate::s_Float32;
FieldFloat64  Aggregate::s_Float64;
FieldInt64    Aggregate::s_Int64;
FieldInt32    Aggregate::s_Int32;
FieldInt16    Aggregate::s_Int16;
FieldInt8     Aggregate::s_Int8;
FieldUInt64   Aggregate::s_UInt64;
FieldUInt32   Aggregate::s_UInt32;
FieldUInt16   Aggregate::s_UInt16;
FieldUInt8    Aggregate::s_UInt8;

void Aggregate::Convert( char* dst_data, const ReadCursor& rc ) const
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    uint32_t name = m_Entry[ i ].m_Name;
    m_Entry[ i ].m_Field->Convert( dst_data + m_Entry[ i ].m_Offset, rc.m_Field->Find( rc.m_Data, name ) );
  }
}

void Aggregate::FixSizeAndStride()
{
  int offset = 0;
  for( int i = 0; i < m_EntryCount; ++i )
  {
    m_Entry[ i ].m_Offset = offset;
    int field_stride = m_Entry[ i ].m_Field->GetElementStride();
    offset += field_stride;
  }
  m_ElementSize = offset;
  m_ElementStride = offset;
}
