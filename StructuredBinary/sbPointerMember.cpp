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

sbPointerMember::sbPointerMember( int count, sbHash type_name )
: sbMember( count, type_name )
{}

size_t sbPointerMember::GetSize() const
{
  return sizeof( void* );
}

size_t sbPointerMember::GetAlignment() const
{
  return __alignof( void* );
}

void sbPointerMember::Convert( char* dst_scope_data, const char* src_scope_data, const sbMember* src_member, sbAllocator* alloc ) const
{
  for( int index = 0; index < GetCount(); ++index )
  {
    char* dst_member_data = GetDataPtr( dst_scope_data, index );
    const char* src_member_data = src_member->GetDataPtr( src_scope_data, index );
    int element_count = src_member->GetPointerCount( src_scope_data, index );
    
    size_t dst_size = GetType()->GetSize();
    size_t src_size = src_member->GetType()->GetSize();
    
    const char* src_p = *( const char** )( src_member_data );
    alloc->StorePointerLocation( src_member_data );
    sbAllocator::Result r = alloc->Alloc( dst_size, element_count, GetType()->GetAlignment(), src_p );
    char* dst_p = r.m_Data;
    if( dst_member_data )
    {
      *( char** )( dst_member_data ) = dst_p;
    }

    if( !r.m_Done )
    {
      for( int j = 0; j < element_count; ++j )
      {
        GetType()->Convert( dst_p, src_p, src_member->GetType(), alloc );
        src_p += src_size;
        dst_p += dst_p ? dst_size : 0;
      }
    }
  }
}

sbStatus sbPointerMember::PreFixUp( sbSchema* schema, sbHash type_name )
{
  return sbStatus_Ok;
}
