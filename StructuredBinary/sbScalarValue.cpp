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

sbScalarValue::sbScalarValue()
{
  m_Type = kNull;
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

sbScalarValue sbScalarValue::UInt( uint64_t value )
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
