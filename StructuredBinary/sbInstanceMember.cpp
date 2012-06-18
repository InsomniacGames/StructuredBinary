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

sbInstanceMember::sbInstanceMember( const sbAggregateType* scope, int count, sbHash type_name )
: sbMember( scope, count, type_name )
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
    GetType()->Convert( dst_member_data, src_member_data, src_member->GetType(), alloc );
  }
}

sbStatus sbInstanceMember::PreFixUp( sbSchema* schema, sbHash type_name )
{
  return schema->FixUp( type_name );
}
