//
//  sbScalarType.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbScalarType.h"

#include <assert.h>

#include "sbStatus.h"

sbScalarType::~sbScalarType()
{}

sbStatus sbScalarType::ConvertOne( char* dst_data, const char* src_data, const sbType* src_type, class sbAllocator* alloc, int array_count ) const
{
  const sbScalarType* src_scalar_type = src_type->AsScalarType();
  sbScalarValue src_value = src_scalar_type->ReadScalarValue( src_data );
  if( dst_data )
  {
    WriteValue( dst_data, src_value );
  }
  return sbStatus_Ok;
}

bool sbScalarType::IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const
{
  return ReadScalarValue( data ) == terminator_value;
}

sbStatus sbScalarType::FixUp( class sbSchema* schema )
{
  return sbStatus_Ok;
}
