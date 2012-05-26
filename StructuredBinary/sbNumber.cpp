//
//  sbNumber.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "sbNumber.h"
// Libraries
// Project

sbNumber::sbNumber()
{
  m_Type = kNull;
}

sbNumber sbNumber::ReinterpretFloatFromU32( uint32_t value )
{
  union
  {
    uint32_t  u32;
    float     f32;
  } convert;
  convert.u32 = value;
  
  sbNumber v;
  v.m_Type = kFloat;
  v.m_Value.f64 = convert.f32;
  return v;
}

sbNumber sbNumber::Float( double value )
{
  sbNumber v;
  v.m_Type = kFloat;
  v.m_Value.f64 = value;
  return v;
}

sbNumber sbNumber::Int( int64_t value )
{
  sbNumber v;
  v.m_Type = kInt;
  v.m_Value.i64 = value;
  return v;
}

sbNumber sbNumber::UInt( uint64_t value )
{
  sbNumber v;
  v.m_Type = kInt;
  v.m_Value.i64 = value;
  return v;
}

sbNumber sbNumber::Null()
{
  sbNumber v;
  v.m_Type = kNull;
  v.m_Value.i64 = 0;
  return v;
}

bool sbNumber::operator== ( const sbNumber& other ) const
{
  return m_Value.i64 == other.m_Value.i64;
}

bool sbNumber::operator!= ( const sbNumber& other ) const
{
  return !( *this == other );
}

double sbNumber::AsFloat() const
{
  switch( m_Type )
  {
    case kFloat:    return m_Value.f64;
    case kInt:      return ( double )m_Value.i64;
    case kNull:     return 0;
  }
}

int64_t sbNumber::AsInt() const
{
  switch( m_Type )
  {
    case kFloat:    return ( int64_t )m_Value.f64;
    case kInt:      return m_Value.i64;
    case kNull:     return 0;
  }
}
