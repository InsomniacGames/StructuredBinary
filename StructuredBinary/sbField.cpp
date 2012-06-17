//
//  sbField.cpp.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbField.h"

#include "sbElement.h"
#include "sbValue.h"

sbField::sbField( const sbAggregate* aggregate, int count, sbHash element_name )
{
  m_Aggregate = aggregate;
  m_Offset = 0;          // To be determined in second pass
  m_Element   = NULL;
  m_ElementName = element_name;
  m_Count  = count;
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

sbValue sbField::ReadValue( const char* aggregate_data ) const
{
  return m_Element->ReadValue( aggregate_data + m_Offset );
}

size_t sbField::GetTotalSize() const
{
  return GetSize() * m_Count;
}
