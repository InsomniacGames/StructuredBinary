//
//  sbStringPointerMember.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbStringPointerMember.h"

#include "sbType.h"
#include "sbValue.h"
#include "sbByteWriter.h"

sbStringPointerMember::sbStringPointerMember( const sbAggregateType* scope, int count, sbHash type_name, sbHash terminator_name, const sbValue& terminator_value )
: sbPointerMember( scope, count, type_name )
{
  m_TerminatorValue = terminator_value;
  m_TerminatorName = terminator_name;
}


int sbStringPointerMember::GetPointerCount( const char* scope_data, int index ) const
{
  const char* member_data = GetDataPtr( scope_data, index );
  
  int string_count = 0;
  const char* p = *( const char** )( member_data );
  bool terminated = false;
  while( !terminated )
  {
    string_count += 1;
    terminated = GetType()->IsTerminal( p, m_TerminatorValue, m_TerminatorName );
    p += GetType()->GetSize();
  }
  return string_count;
}

void sbStringPointerMember::Write( sbByteWriter* writer )
{
  if( GetCount() == 1 )
  {
    writer->Write8( sbMemberType_StringPointer );
    writer->Write32( GetTypeName() );
    writer->Write32( m_TerminatorName );
    m_TerminatorValue.Write( writer );
  }
  else
  {
    writer->Write8( sbMemberType_StringPointerArray );
    writer->Write32( GetTypeName() );
    writer->Write16( GetCount() );
    writer->Write32( m_TerminatorName );
    m_TerminatorValue.Write( writer );
  }
}
