//
//  sbStringPointerMember.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbStringPointerMember.h"

#include "sbType.h"
#include "sbScalarValue.h"
#include "sbByteWriter.h"
#include "sbByteReader.h"

sbStringPointerMember::sbStringPointerMember( int count, sbHash type_name, sbHash terminator_name, const sbScalarValue& terminator_value )
: sbPointerMember( count, "*", type_name )
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
    terminated = GetIndirectType()->IsTerminal( p, m_TerminatorValue, m_TerminatorName );
    p += GetIndirectType()->GetSize();
  }
  return string_count;
}

void sbStringPointerMember::Write( sbByteWriter* writer ) const
{
  uint8_t code = ByteCode_StringPointer;
  
  if( GetCount() > 1 )
  {
    code |= ByteCodeFlag_Array;
  }
  if( m_TerminatorName )
  {
    code |= ByteCodeFlag_TermName;
  }
  if( !m_TerminatorValue.IsZero() )
  {
    code |= ByteCodeFlag_TermValue;
  }
  
  writer->Write8( code );
  writer->Write32( GetIndirectTypeName() );

  if( code & ByteCodeFlag_Array )
  {
    writer->Write16( GetCount() );
  }
  if( code & ByteCodeFlag_TermName )
  {
    writer->Write32( m_TerminatorName );
  }
  if( code & ByteCodeFlag_TermValue )
  {
    writer->Write( &m_TerminatorValue );
  }
}

sbStringPointerMember* sbStringPointerMember::ReadNew( sbByteReader* reader )
{
  size_t roll_back = reader->Tell();
  sbStringPointerMember* member = NULL;
  
  uint8_t code = reader->Read8();
  
  if( ( code & ByteCode_Mask ) == ByteCode_StringPointer )
  {
    int count = 1;
    sbHash terminator_name;
    sbScalarValue terminator_value;

    sbHash type_name = reader->Read32();
    if( code & ByteCodeFlag_Array )
    {
      count = reader->Read16();
    }
    if( code & ByteCodeFlag_TermName )
    {
      terminator_name = reader->Read32();
    }
    if( code & ByteCodeFlag_TermValue )
    {
      terminator_value = reader->Read< sbScalarValue >();
    }

    member = new sbStringPointerMember( count, type_name, terminator_name, terminator_value );
  }
  
  if( !member )
  {
    reader->Seek( roll_back );
  }
  return member;
}

uint64_t sbStringPointerMember::GetChecksum( uint64_t basis ) const
{
  basis = sbMember::GetChecksum( basis );
  basis = sbFnv64( basis, m_TerminatorName );
  basis = m_TerminatorValue.GetChecksum( basis );
  return basis;
}

