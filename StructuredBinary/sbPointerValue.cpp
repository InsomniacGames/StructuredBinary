//
//  sbPointerValue.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/24/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbPointerValue.h"

#include <string>

sbPointerValue::sbPointerValue()
: m_Value( NULL )
{}

sbPointerValue sbPointerValue::Pointer( const char* p )
{
  sbPointerValue value;
  value.m_Value = p;
  return value;
}

const char* sbPointerValue::AsConstCharStar() const
{
  return m_Value;
}

char* sbPointerValue::AsCharStar() const
{
  return ( char * )m_Value;
}
