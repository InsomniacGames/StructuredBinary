//
//  sbCountedPointerMember.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbCountedPointerMember.h"

#include "sbAggregateType.h"
#include "sbByteWriter.h"

sbCountedPointerMember::sbCountedPointerMember( const sbAggregateType* scope, int count, sbHash type_name, sbHash count_name )
: sbPointerMember( scope, count, type_name )
{
  m_CountName = count_name;
}

int sbCountedPointerMember::GetPointerCount( const char* scope_data, int index ) const
{
  int pointer_count = 1;
  const sbMember* count_member = GetScope()->FindMember( m_CountName );
  if( count_member )
  {
    const char* count_member_data = count_member->GetDataPtr( scope_data, 0 );
    pointer_count = ( int )count_member->GetType()->ReadValue( count_member_data ).AsInt();
  }
  return pointer_count;
}

void sbCountedPointerMember::Write( sbByteWriter* writer )
{
  if( GetCount() == 1 )
  {
    writer->Write8( sbMemberType_CountedPointer );
    writer->Write32( GetTypeName() );
    writer->Write32( m_CountName );
  }
  else
  {
    writer->Write8( sbMemberType_CountedPointerArray );
    writer->Write32( GetTypeName() );
    writer->Write16( GetCount() );
    writer->Write32( m_CountName );
  }
}
