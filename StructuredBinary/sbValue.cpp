//
//  sbValue.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "sbValue.h"
// Libraries
// Project

sbValue::sbValue()
{
  m_Type = kNull;
}

sbValue sbValue::ReinterpretFloatFromU32( uint32_t value )
{
  union
  {
    uint32_t  u32;
    float     f32;
  } convert;
  convert.u32 = value;
  
  sbValue v;
  v.m_Type = kFloat;
  v.m_Value.f64 = convert.f32;
  return v;
}

sbValue sbValue::Float( double value )
{
  sbValue v;
  v.m_Type = kFloat;
  v.m_Value.f64 = value;
  return v;
}

sbValue sbValue::Int( int64_t value )
{
  sbValue v;
  v.m_Type = kInt;
  v.m_Value.i64 = value;
  return v;
}

sbValue sbValue::Null()
{
  sbValue v;
  v.m_Type = kNull;
  v.m_Value.i64 = 0;
  return v;
}

bool sbValue::operator== ( const sbValue& other ) const
{
  return m_Value.i64 == other.m_Value.i64;
}

bool sbValue::operator!= ( const sbValue& other ) const
{
  return !( *this == other );
}

double sbValue::AsFloat() const
{
  switch( m_Type )
  {
    case kFloat:    return m_Value.f64;
    case kInt:      return ( double )m_Value.i64;
    case kNull:     return 0;
  }
}

int64_t sbValue::AsInt() const
{
  switch( m_Type )
  {
    case kFloat:    return ( int64_t )m_Value.f64;
    case kInt:      return m_Value.i64;
    case kNull:     return 0;
  }
}
