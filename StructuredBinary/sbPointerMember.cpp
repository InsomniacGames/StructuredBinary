//
//  sbPointerMember.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbPointerMember.h"

#include "sbUtil.h"
#include "sbType.h"
#include "sbAllocator.h"

#include "sbStatus.h"
#include "sbSchema.h"

#include "sbByteWriter.h"
#include "sbByteReader.h"

sbPointerMember::sbPointerMember( int count, sbHash type_name )
: sbMember( count, type_name )
, m_Type( NULL )
{}

sbPointerMember::~sbPointerMember()
{
  delete m_Type;
}

size_t sbPointerMember::GetSize() const
{
  return GetType()->GetSize();
}

size_t sbPointerMember::GetAlignment() const
{
  return GetType()->GetAlignment();
}

void sbPointerMember::Convert( char* dst_scope_data, const char* src_scope_data, const sbMember* src_member, sbAllocator* alloc ) const
{
  for( int index = 0; index < GetCount(); ++index )
  {
    char* dst_member_data = GetDataPtr( dst_scope_data, index );
    const char* src_member_data = src_member->GetDataPtr( src_scope_data, index );
    int count = src_member->GetPointerCount( src_scope_data, index );
    GetType()->ConvertOne( dst_member_data, src_member_data, src_member->GetType(), alloc, count );
  }
}

const sbType* sbPointerMember::PreFixUp( sbSchema* schema, sbHash type_name )
{
  const sbType* indirect_type = schema->FindType( type_name );
  m_Type = new sbPointerType( indirect_type );
  return m_Type;
}
