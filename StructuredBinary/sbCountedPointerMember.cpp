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
#include "sbByteReader.h"

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

void sbCountedPointerMember::Write( sbByteWriter* writer ) const
{
  uint8_t code = ByteCode_CountedPointer;

  if( GetCount() > 1 )
  {
    code |= ByteCodeFlag_Array;
  }

  if( m_CountName )
  {
    code |= ByteCodeFlag_CountName;
  }

  writer->Write8( code );
  writer->Write32( GetTypeName() );
  if( code & ByteCodeFlag_Array )
  {
    writer->Write16( GetCount() );
  }
  if( code & ByteCodeFlag_CountName )
  {
    writer->Write32( m_CountName );
  }
}

sbMember* sbCountedPointerMember::Read( sbByteReader* reader, const sbAggregateType* scope )
{
  size_t roll_back = reader->Tell();
  sbMember* member = NULL;
  
  uint8_t code = reader->Read8();

  if( ( code & ByteCode_Mask ) == ByteCode_CountedPointer )
  {
    int count = 1;
    sbHash count_name = 0U;

    sbHash type_name = reader->Read32();
    if( code & ByteCodeFlag_Array )
    {
      count = reader->Read16();
    }
    if( code & ByteCodeFlag_CountName )
    {
      count_name = reader->Read32();
    }
    
    member = new sbCountedPointerMember( scope, count, type_name, count_name );
  }
  
  if( !member )
  {
    reader->Seek( roll_back );
  }
  return member;
}

