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

/*
 Note to self:
 It should be possible to implement a ReadPointerValue here.
 At this point we have access to the scope, and can therefore read the count.
 The sbPointerValue needs to be changed to contain an sbBlock
 */

void sbPointerMember::Convert( char* dst_scope_data, const char* src_scope_data, const sbMember* src_member, sbAllocator* alloc ) const
{
  // TO DO: replace this with one ConvertMany of InlineType. The ConvertOne for the pointer type will involve memory allocation.
  // Then iterate over all allocated pointers, and call ConvertMany on each of them.

  for( int index = 0; index < GetCount(); ++index )
  {
    char* dst_member_data = GetDataPtr( dst_scope_data, index );
    const char* src_member_data = src_member->GetDataPtr( src_scope_data, index );
    int count = src_member->GetPointerCount( src_scope_data, index );
    GetType()->ConvertOne( dst_member_data, src_member_data, src_member->GetType(), alloc, count );
/*
    char* dst_member_data = GetDataPtr( dst_scope_data, index );
    const char* src_member_data = src_member->GetDataPtr( src_scope_data, index );
    int count = src_member->GetPointerCount( src_scope_data, index );


    const char* src_p = *( const char** )( src_member_data );
    alloc->StorePointerLocation( src_member_data );

    char* dst_p = alloc->Alloc( GetIndirectType(), src_member->GetIndirectType(), src_p, src_count );

    if( dst_member_data )
    {
      *( char** )( dst_member_data ) = dst_p;
    }
*/
  }
}

const sbType* sbPointerMember::PreFixUp( sbSchema* schema, sbHash type_name )
{
  const sbType* indirect_type = schema->FindType( type_name );
  m_Type = new sbPointerType( indirect_type );
  return m_Type;
}
