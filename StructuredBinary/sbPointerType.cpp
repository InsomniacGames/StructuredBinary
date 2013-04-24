//
//  sbPointerType.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/24/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbPointerType.h"

#include <assert.h>
#include <stdio.h>

#include "sbStatus.h"
#include "sbPointerValue.h"
#include "sbSchema.h"

sbPointerType::sbPointerType( const sbType* indirect_type )
: m_IndirectType( indirect_type )
{}

sbStatus sbPointerType::ConvertOne( char* dst_data, const char* src_data, const sbType* src_type, class sbAllocator* alloc, int array_count ) const
{
  const sbPointerType* src_pointer_type = src_type->AsPointerType();

  sbPointerValue pointer_value = src_pointer_type->ReadPointerValue( src_data );
  const char* src_p = pointer_value.AsConstCharStar();
  alloc->StorePointerLocation( src_data );

  // TO DO: I think the Alloc belongs in the PointerMember. Then I don't have to pass the array_count around, which has ambiguous meaning at the sbType level anyway
  // That means ReadPointerValue needs to go into sbPointerMember. I think that makes sense.
  // Also, both ReadPointerValue and ReadScalarValue in sbMember should take an index, because members can be arrays
  char* dst_p = alloc->Alloc( GetIndirectType(), src_pointer_type->GetIndirectType(), src_p, array_count );
  
  if( dst_data )
  {
    sbPointerValue dst_pointer_value = sbPointerValue::Pointer( dst_p );
    WritePointerValue( dst_data, dst_pointer_value );
  }

  return sbStatus_Ok;
}

size_t sbPointerType::GetSize() const
{
  return sizeof( void* );
}

size_t sbPointerType::GetAlignment() const
{
  return __alignof( void* );
}

bool sbPointerType::IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const
{
  // terminator_value is ignored. The only terminating value for pointers is NULL
  sbPointerValue pointer_value = ReadPointerValue( data );
  const char* p = pointer_value.AsConstCharStar();
  return p == NULL;
}

sbPointerValue sbPointerType::ReadPointerValue( const char* data ) const
{
  return sbPointerValue::Pointer( *( const char** )( data ) );
}

void sbPointerType::WritePointerValue( char* data, const sbPointerValue& value ) const
{
  *( const char** )( data ) = value.AsConstCharStar();
}

sbStatus sbPointerType::FixUp( class sbSchema* schema )
{
  return sbStatus_Ok;
}
