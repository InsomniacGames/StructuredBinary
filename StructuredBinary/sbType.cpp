//
//  sbType.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/23/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbType.h"

#include <assert.h>

#include "sbAggregateType.h"
#include "sbByteReader.h"
#include "sbPointerValue.h"
#include "sbScalarValue.h"

sbType* sbType::ReadNew( sbByteReader* reader )
{
  sbType* sb_type = NULL;
  if( !sb_type ) sb_type = reader->ReadNew< sbAggregateType >();
  return sb_type;
}

sbScalarValue sbType::ReadScalarValue( const char* data ) const
{
  // TO DO: probably should report error and bail out
  assert( false );
}

sbPointerValue sbType::ReadPointerValue( const char* data ) const
{
  // TO DO: probably should report error and bail out
  assert( false );
}

const class sbMember* sbType::FindMember( sbHash name ) const
{
  // TO DO: probably should report error and bail out
  assert( false );
}

sbStatus sbType::ConvertMany( const sbDestArray& dst_array, const sbSourceArray& src_array, class sbAllocator* alloc )
{
  char* dst_data = dst_array.m_Data;
  const char* src_data = src_array.m_Data;

  size_t dst_stride = dst_data ? dst_array.m_Type->GetSize() : 0;
  size_t src_stride = src_data ? src_array.m_Type->GetSize() : 0;

  for( int i = 0; i < dst_array.m_Count; ++i )
  {
    dst_array.m_Type->ConvertOne( dst_data, src_data, src_array.m_Type, alloc );
    dst_data += dst_stride;
    src_data += src_stride;
  }

  return sbStatus_Ok;
}
