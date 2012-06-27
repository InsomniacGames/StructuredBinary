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
#include "sbHash.h"

sbCountedPointerMember::sbCountedPointerMember( int count, sbHash type_name, sbHash count_name )
: sbPointerMember( count, "*", type_name )
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
    pointer_count = ( int )count_member->GetInlineType()->ReadScalarValue( count_member_data ).AsInt();
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
  writer->Write32( GetIndirectTypeName() );
  if( code & ByteCodeFlag_Array )
  {
    writer->Write16( GetCount() );
  }
  if( code & ByteCodeFlag_CountName )
  {
    writer->Write32( m_CountName );
  }
}

sbCountedPointerMember* sbCountedPointerMember::ReadNew( sbByteReader* reader )
{
  size_t roll_back = reader->Tell();
  sbCountedPointerMember* member = NULL;
  
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
    
    member = new sbCountedPointerMember( count, type_name, count_name );
  }
  
  if( !member )
  {
    reader->Seek( roll_back );
  }
  return member;
}

uint64_t sbCountedPointerMember::GetChecksum( uint64_t basis ) const
{
  basis = sbMember::GetChecksum( basis );
  basis = sbFnv64( basis, m_CountName );
  return basis;
}

