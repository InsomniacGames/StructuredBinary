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

#include "sbByteWriter.h"
#include "sbByteReader.h"

sbPointerMember::sbPointerMember( int count, sbHash type_name, sbHash indirect_type_name )
: sbMember( count, type_name, indirect_type_name )
{}

size_t sbPointerMember::GetSize() const
{
  return GetInlineType()->GetSize();
}

size_t sbPointerMember::GetAlignment() const
{
  return GetInlineType()->GetAlignment();
}

void sbPointerMember::Convert( char* dst_scope_data, const char* src_scope_data, const sbMember* src_member, sbAllocator* alloc ) const
{
  // TO DO: replace this with one ConvertMany of InlineType. The ConvertOne for the pointer type will involve memory allocation.
  // Then iterate over all allocated pointers, and call ConvertMany on each of them.

  for( int index = 0; index < GetCount(); ++index )
  {
    char* dst_member_data = GetDataPtr( dst_scope_data, index );
    const char* src_member_data = src_member->GetDataPtr( src_scope_data, index );
    int src_count = src_member->GetPointerCount( src_scope_data, index );

    const char* src_p = *( const char** )( src_member_data );
    alloc->StorePointerLocation( src_member_data );

    char* dst_p = alloc->Alloc( GetIndirectType(), src_member->GetIndirectType(), src_p, src_count );

    if( dst_member_data )
    {
      *( char** )( dst_member_data ) = dst_p;
    }
  }
}

sbStatus sbPointerMember::PreFixUp( sbSchema* schema, sbHash type_name )
{
  return sbStatus_Ok;
}
