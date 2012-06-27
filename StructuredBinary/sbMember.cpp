//
//  sbMember.cpp.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbMember.h"

#include "sbType.h"
#include "sbScalarValue.h"
#include "sbSchema.h"
#include "sbUtil.h"
#include "sbByteReader.h"

#include "sbInstanceMember.h"
#include "sbPointerMember.h"
#include "sbCountedPointerMember.h"
#include "sbStringPointerMember.h"

sbMember::sbMember( int count, sbHash inline_type_name, sbHash indirect_type_name )
{
  m_Scope = NULL;
  m_Offset = 0;          // To be determined in second pass
  m_InlineType   = NULL;
  m_IndirectType  = NULL;
  m_InlineTypeName = inline_type_name;
  m_IndirectTypeName = indirect_type_name;
  m_Count  = count;
}

sbMember::~sbMember()
{}

void sbMember::SetScope( const sbAggregateType* scope )
{
  m_Scope = scope;
}

char* sbMember::GetDataPtr( char* scope_data, int index ) const
{
  return scope_data ? scope_data + m_Offset + index * GetSize() : NULL;
}

const char* sbMember::GetDataPtr( const char* scope_data, int index ) const
{
  return scope_data ? scope_data + m_Offset + index * GetSize() : NULL;
}

sbScalarValue sbMember::ReadScalarValue( const char* scope_data ) const
{
  return m_InlineType->ReadScalarValue( scope_data + m_Offset );
}

size_t sbMember::GetTotalSize() const
{
  return GetSize() * m_Count;
}

sbStatus sbMember::FixUp( sbSchema* schema, const sbMember* previous_member )
{
  sbStatus status = PreFixUp( schema, m_InlineTypeName );
  
  if( status == sbStatus_Ok )
  {
    m_InlineType = schema->FindType( m_InlineTypeName );
    if( !m_InlineType )
    {
      status = sbStatus_ErrorNodeNotFound;
    }
  }
  
  if( status == sbStatus_Ok )
  {
    if( m_IndirectTypeName )
    {
      m_IndirectType = schema->FindType( m_IndirectTypeName );
      if( !m_IndirectType )
      {
        status = sbStatus_ErrorNodeNotFound;
      }
    }
  }
  
  if( status == sbStatus_Ok )
  {
    size_t offset = previous_member ? previous_member->m_Offset + previous_member->GetTotalSize() : 0;
    offset = FIX_ALIGNMENT( offset, GetAlignment() );
    m_Offset = offset;
  }
  
  return status;
}

sbMember* sbMember::ReadNew( sbByteReader* reader )
{
  sbMember* member = NULL;
  if( !member ) member = reader->ReadNew< sbInstanceMember >();
  if( !member ) member = reader->ReadNew< sbCountedPointerMember >();
  if( !member ) member = reader->ReadNew< sbStringPointerMember >();
  return member;
}

uint64_t sbMember::GetChecksum( uint64_t basis ) const
{
  basis = sbFnv64( basis, m_InlineTypeName );
  basis = sbFnv64( basis, m_Offset );
  basis = sbFnv64( basis, m_Count );
  return basis;
}
