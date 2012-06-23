//
//  sbScalarValue.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "sbScalarValue.h"
// Libraries
// Project
#include "sbByteWriter.h"
#include "sbByteReader.h"

sbScalarValue::sbScalarValue()
{
  m_Type = kNull;
  m_Value.i64 = 0;
}

sbScalarValue sbScalarValue::ReinterpretFloatFromU32( uint32_t value )
{
  union
  {
    uint32_t  u32;
    float     f32;
  } convert;
  convert.u32 = value;
  
  sbScalarValue v;
  v.m_Type = kFloat;
  v.m_Value.f64 = convert.f32;
  return v;
}

sbScalarValue sbScalarValue::Float( double value )
{
  sbScalarValue v;
  v.m_Type = kFloat;
  v.m_Value.f64 = value;
  return v;
}

sbScalarValue sbScalarValue::Int( int64_t value )
{
  sbScalarValue v;
  v.m_Type = kInt;
  v.m_Value.i64 = value;
  return v;
}

sbScalarValue sbScalarValue::Null()
{
  sbScalarValue v;
  v.m_Type = kNull;
  v.m_Value.i64 = 0;
  return v;
}

bool sbScalarValue::operator== ( const sbScalarValue& other ) const
{
  return m_Value.i64 == other.m_Value.i64;
}

bool sbScalarValue::operator!= ( const sbScalarValue& other ) const
{
  return !( *this == other );
}

double sbScalarValue::AsFloat() const
{
  switch( m_Type )
  {
    case kFloat:    return m_Value.f64;
    case kInt:      return ( double )m_Value.i64;
    case kNull:     return 0;
  }
}

int64_t sbScalarValue::AsInt() const
{
  switch( m_Type )
  {
    case kFloat:    return ( int64_t )m_Value.f64;
    case kInt:      return m_Value.i64;
    case kNull:     return 0;
  }
}

void sbScalarValue::Write( sbByteWriter* writer ) const
{
  writer->Write8( m_Type );
  switch( m_Type )
  {
    case kFloat:
    {
      union
      {
        uint32_t  u32;
        float     f32;
      } convert;
      convert.f32 = ( float )m_Value.f64;
      writer->Write32( convert.u32 );
      break;
    }
    case kInt:
    {
      writer->Write32( ( int32_t )m_Value.i64 );
      break;
    }
    case kNull:
    {
      break;
    }
  }
}

sbScalarValue sbScalarValue::Read( sbByteReader* reader )
{
  Type t = ( Type )reader->Read8();
  switch( t )
  {
    case kFloat:
    {
      union
      {
        uint32_t  u32;
        float     f32;
      } convert;
      convert.u32 = reader->Read32();
      return sbScalarValue::Float( convert.f32 );
    }
    case kInt:
    {
      int32_t i32 = reader->Read32();
      return sbScalarValue::Int( i32 );
      break;
    }
    case kNull:
    {
      return sbScalarValue::Null();
      break;
    }
  }
}

