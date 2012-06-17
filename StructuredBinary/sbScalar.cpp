//
//  sbScalar.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbScalar.h"

#include "sbStatus.h"

sbScalar::~sbScalar()
{}

sbStatus sbScalar::Convert( char* dst_element_data, const char* src_element_data, const sbElement* src_element, class sbAllocator* alloc ) const
{
  sbValue src_value = src_element->ReadValue( src_element_data );
  if( dst_element_data )
  {
    WriteValue( dst_element_data, src_value );
  }
  return sbStatus_Ok;
}

bool sbScalar::IsTerminal( const char* data, const sbValue& terminator_value, sbHash terminator_name ) const
{
  return ReadValue( data ) == terminator_value;
}

sbStatus sbScalar::FixUp( class sbSchema* schema )
{
  return sbStatus_Ok;
}

const sbField* sbScalar::FindField( sbHash name ) const
{
  return NULL;
}
