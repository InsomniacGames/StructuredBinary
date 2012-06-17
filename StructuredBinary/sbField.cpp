//
//  sbField.cpp.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbField.h"

sbField::sbField( const sbAggregate* aggregate, sbHash field_name, int count, sbHash element_name )
{
  m_Aggregate = aggregate;
  m_Offset = 0;          // To be determined in second pass
  m_Element   = NULL;
  m_FieldName   = field_name;
  m_ElementName = element_name;
  m_Count  = count;
  m_Next = NULL;
}

sbField::~sbField()
{}

char* sbField::GetDataPtr( char* aggregate_data, int index ) const
{
  return aggregate_data ? aggregate_data + m_Offset + index * GetSize() : NULL;
}
const char* sbField::GetDataPtr( const char* aggregate_data, int index ) const
{
  return aggregate_data ? aggregate_data + m_Offset + index * GetSize() : NULL;
}
