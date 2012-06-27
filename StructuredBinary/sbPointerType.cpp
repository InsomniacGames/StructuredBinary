//
//  sbPointerType.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/24/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbPointerType.h"

#include <assert.h>

#include "sbStatus.h"
#include "sbPointerValue.h"

sbStatus sbPointerType::ConvertOne( char* dst_data, const char* src_data, const sbType* src_type, class sbAllocator* alloc ) const
{
  sbPointerValue src_value = src_type->ReadPointerValue( src_data );
  if( dst_data )
  {
    WriteValue( dst_data, src_value );
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

sbStatus sbPointerType::FixUp( class sbSchema* schema )
{
  return sbStatus_Ok;
}

sbPointerValue sbPointerType::ReadPointerValue( const char* data ) const
{
  return sbPointerValue::Pointer( *( const char** )( data ) );
}

void sbPointerType::WriteValue( char* data, const sbPointerValue& value ) const
{
  *( const char** )( data ) = value.AsConstCharStar();
}
