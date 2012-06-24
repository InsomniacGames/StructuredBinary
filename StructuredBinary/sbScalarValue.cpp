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
#include "stdio.h"
// Project
#include "sbByteWriter.h"
#include "sbByteReader.h"
#include "sbHash.h"

sbScalarValue::sbScalarValue()
{
  m_Type = kZero;
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
  if( value != 0.0 )
  {
    v.m_Type = kFloat;
    v.m_Value.f64 = value;
  }
  return v;
}

sbScalarValue sbScalarValue::Int( int64_t value )
{
  sbScalarValue v;
  if( value != 0 )
  {
    v.m_Type = kInt;
    v.m_Value.i64 = value;
  }
  return v;
}

sbScalarValue sbScalarValue::Zero()
{
  return sbScalarValue();
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
    case kZero:     return 0;
  }
}

int64_t sbScalarValue::AsInt() const
{
  switch( m_Type )
  {
    case kFloat:    return ( int64_t )m_Value.f64;
    case kInt:      return m_Value.i64;
    case kZero:     return 0;
  }
}

void sbScalarValue::Write( sbByteWriter* writer ) const
{
  writer->Write8( m_Type );
  switch( m_Type )
  {
    case kInt:
    case kFloat:
    {
      writer->Write64( m_Value.i64 );
      break;
    }
    case kZero:
    {
      break;
    }
  }
}

sbScalarValue sbScalarValue::Read( sbByteReader* reader )
{
  sbScalarValue v;
  Type t = ( Type )reader->Read8();
  
  v.m_Type = t;
  switch( t )
  {
    case kInt:
    case kFloat:
    {
      v.m_Value.i64 = reader->Read64();
    }
    case kZero:
    {
      break;
    }
  }
  return v;
}

uint64_t sbScalarValue::GetChecksum( uint64_t basis ) const
{
  basis = sbFnv64( basis, m_Type );
  basis = sbFnv64( basis, m_Value.i64 );
  return basis;
}

