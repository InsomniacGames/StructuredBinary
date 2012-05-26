//
//  Field.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include <assert.h>

#include "Field.h"

static const Field* BuildF32()
{
  return new FieldFloat32;
}

static const Field* BuildF64()
{
  return new FieldFloat64;
}

static const Field* BuildI64()
{
  return new FieldInt64;
}

static const Field* BuildI32()
{
  return new FieldInt32;
}

static const Field* BuildI16()
{
  return new FieldInt16;
}

static const Field* BuildI8()
{
  return new FieldInt8;
}

static const Field* BuildU64()
{
  return new FieldUInt64;
}

static const Field* BuildU32()
{
  return new FieldUInt32;
}

static const Field* BuildU16()
{
  return new FieldUInt16;
}

static const Field* BuildU8()
{
  return new FieldUInt8;
}

static const Field* BuildUnknown()
{
  assert( false );
  return NULL;
}

const FieldInfo g_FieldInfo[ kField_Count ] =
{
  { kField_Unknown, "unknown",  &BuildUnknown },
  { kField_I8,      "int8_t",   &BuildI8      },
  { kField_U8,      "uint8_t",  &BuildU8      },
  { kField_I16,     "int16_t",  &BuildI16     },
  { kField_U16,     "uint16_t", &BuildU16     },
  { kField_I32,     "int32_t",  &BuildI32     },
  { kField_U32,     "uint32_t", &BuildU32     },
  { kField_I64,     "int64_t",  &BuildI64     },
  { kField_U64,     "uint64_t", &BuildU64     },
  { kField_F32,     "float",    &BuildF32     },
  { kField_F64,     "double",   &BuildF64     },
  { kField_Agg,     "agg",      &BuildUnknown },
};

const FieldInfo& GetInfo( FieldType field_type )
{
  assert( field_type >= 0 && field_type < kField_Count );
  assert( g_FieldInfo[ field_type ].field_type == field_type );
  return g_FieldInfo[ field_type ];
}

