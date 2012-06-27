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
#include "sbBlock.h"

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

sbStatus sbType::ConvertMany( const sbBlock* block, class sbAllocator* alloc )
{
  char* dst_data = block->GetDstPtr();
  const char* src_data = block->m_SrcData;

  size_t dst_stride = dst_data ? block->m_DstType->GetSize() : 0;
  size_t src_stride = src_data ? block->m_SrcType->GetSize() : 0;

  for( int i = 0; i < block->m_Count; ++i )
  {
    block->m_DstType->ConvertOne( dst_data, src_data, block->m_SrcType, alloc, 1 );
    dst_data += dst_stride;
    src_data += src_stride;
  }

  return sbStatus_Ok;
}
