//
//  sbField.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include <assert.h>

#include "sbField.h"
#include "sbStruct.h"

static const sbField* BuildF32( sbByteReader* reader )
{
  return new FieldFloat32;
}

static const sbField* BuildF64( sbByteReader* reader )
{
  return new FieldFloat64;
}

static const sbField* BuildI64( sbByteReader* reader )
{
  return new FieldInt64;
}

static const sbField* BuildI32( sbByteReader* reader )
{
  return new FieldInt32;
}

static const sbField* BuildI16( sbByteReader* reader )
{
  return new FieldInt16;
}

static const sbField* BuildI8( sbByteReader* reader )
{
  return new FieldInt8;
}

static const sbField* BuildU64( sbByteReader* reader )
{
  return new FieldUInt64;
}

static const sbField* BuildU32( sbByteReader* reader )
{
  return new FieldUInt32;
}

static const sbField* BuildU16( sbByteReader* reader )
{
  return new FieldUInt16;
}

static const sbField* BuildU8( sbByteReader* reader )
{
  return new FieldUInt8;
}

static const sbField* BuildUnknown( sbByteReader* reader )
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
  { kField_Agg,     "agg",      &sbStruct::BuildAgg },
};

const FieldInfo& GetInfo( FieldType field_type )
{
  assert( field_type >= 0 && field_type < kField_Count );
  assert( g_FieldInfo[ field_type ].field_type == field_type );
  return g_FieldInfo[ field_type ];
}

