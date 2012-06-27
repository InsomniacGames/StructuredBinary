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
  const char* src_p = *( const char** )( src_data );
  alloc->StorePointerLocation( src_data );
  
  char* dst_p = alloc->Alloc( GetIndirectType(), src_type->GetIndirectType(), src_p, array_count );
  
  if( dst_data )
  {
    *( char** )( dst_data ) = dst_p;
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
  const char* p = *( const char** )( data );
  return p == NULL;
}

sbPointerValue sbPointerType::ReadPointerValue( const char* data ) const
{
  return sbPointerValue::Pointer( *( const char** )( data ) );
}

void sbPointerType::WriteValue( char* data, const sbPointerValue& value ) const
{
  *( const char** )( data ) = value.AsConstCharStar();
}

sbStatus sbPointerType::FixUp( class sbSchema* schema )
{
  return sbStatus_Ok;
}
