//
//  sbStringPointerField.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbStringPointerField.h"

#include "sbElement.h"

sbStringPointerField::sbStringPointerField( const sbAggregate* aggregate, int count, sbHash element_name, sbHash terminator_name, const sbValue& terminator_value )
: sbPointerField( aggregate, count, element_name )
{
  m_TerminatorValue = terminator_value;
  m_TerminatorName = terminator_name;
}

int sbStringPointerField::GetPointerCount( const char* aggregate_data, int index ) const
{
  const char* field_data = aggregate_data + m_Offset + index * GetSize();
  
  int string_count = 0;
  const char* p = *( const char** )( field_data );
  bool terminated = false;
  while( !terminated )
  {
    string_count += 1;
    terminated = m_Element->IsTerminal( p, m_TerminatorValue, m_TerminatorName );
    p += m_Element->GetSize();
  }
  return string_count;
}
