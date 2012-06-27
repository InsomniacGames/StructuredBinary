//
//  sbInstanceMember.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbInstanceMember.h"
#include "sbUtil.h"
#include "sbType.h"
#include "sbMember.h"
#include "sbSchema.h"

#include "sbByteWriter.h"
#include "sbByteReader.h"

sbInstanceMember::sbInstanceMember( int count, sbHash type_name )
: sbMember( count, type_name )
{}

size_t sbInstanceMember::GetSize() const
{
  return GetType()->GetSize();
}

size_t sbInstanceMember::GetAlignment() const
{
  return GetType()->GetAlignment();
}

int sbInstanceMember::GetPointerCount( const char* scope_data, int index ) const
{
  return 0;
}

void sbInstanceMember::Convert( char* dst_scope_data, const char* src_scope_data, const sbMember* src_member, sbAllocator* alloc ) const
{
  for( int index = 0; index < GetCount(); ++index )
  {
    char* dst_member_data = GetDataPtr( dst_scope_data, index );
    const char* src_member_data = src_member->GetDataPtr( src_scope_data, index );
    GetType()->ConvertOne( dst_member_data, src_member_data, src_member->GetType(), alloc, 1 );
  }
}

const sbType* sbInstanceMember::PreFixUp( sbSchema* schema, sbHash type_name )
{
  schema->FixUp( type_name );
  return schema->FindType( type_name );
}

void sbInstanceMember::Write( sbByteWriter* writer ) const
{
  uint8_t code = ByteCode_Instance;
  
  if( GetCount() > 1 )
  {
    code |= ByteCodeFlag_Array;
  }

  writer->Write8( code );
  writer->Write32( GetTypeName() );
  if( code & ByteCodeFlag_Array )
  {
    writer->Write16( GetCount() );
  }
}

sbInstanceMember* sbInstanceMember::ReadNew( sbByteReader* reader )
{
  size_t roll_back = reader->Tell();
  sbInstanceMember* member = NULL;
  
  uint8_t code = reader->Read8();
  
  if( ( code & ByteCode_Mask ) == ByteCode_Instance )
  {
    int count = 1;
    
    sbHash type_name = reader->Read32();
    if( code & ByteCodeFlag_Array )
    {
      count = reader->Read16();
    }
    
    member = new sbInstanceMember( count, type_name );
  }
  
  if( !member )
  {
    reader->Seek( roll_back );
  }
  return member;
}

