//
//  Number.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "Number.h"
// Libraries
// Project

Number::Number()
{
  m_Type = kNull;
}

Number Number::ReinterpretFloatFromU32( uint32_t value )
{
  union
  {
    uint32_t  u32;
    float     f32;
  } convert;
  convert.u32 = value;
  
  Number v;
  v.m_Type = kFloat;
  v.m_Value.f64 = convert.f32;
  return v;
}

Number Number::Float( double value )
{
  Number v;
  v.m_Type = kFloat;
  v.m_Value.f64 = value;
  return v;
}

Number Number::Int( int64_t value )
{
  Number v;
  v.m_Type = kInt;
  v.m_Value.i64 = value;
  return v;
}

Number Number::UInt( uint64_t value )
{
  Number v;
  v.m_Type = kInt;
  v.m_Value.i64 = value;
  return v;
}

Number Number::Null()
{
  Number v;
  v.m_Type = kNull;
  v.m_Value.i64 = 0;
  return v;
}

bool Number::operator== ( const Number& other ) const
{
  return m_Value.i64 == other.m_Value.i64;
}

bool Number::operator!= ( const Number& other ) const
{
  return !( *this == other );
}

double Number::AsFloat() const
{
  switch( m_Type )
  {
    case kFloat:    return m_Value.f64;
    case kInt:      return ( double )m_Value.i64;
    case kNull:     return 0;
  }
}

int64_t Number::AsInt() const
{
  switch( m_Type )
  {
    case kFloat:    return ( int64_t )m_Value.f64;
    case kInt:      return m_Value.i64;
    case kNull:     return 0;
  }
}
