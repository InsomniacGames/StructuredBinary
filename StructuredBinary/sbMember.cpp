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

sbMember::sbMember( const sbAggregateType* scope, int count, sbHash type_name )
{
  m_Scope = scope;
  m_Offset = 0;          // To be determined in second pass
  m_Type   = NULL;
  m_TypeName = type_name;
  m_Count  = count;
}

sbMember::~sbMember()
{}

char* sbMember::GetDataPtr( char* scope_data, int index ) const
{
  return scope_data ? scope_data + m_Offset + index * GetSize() : NULL;
}

const char* sbMember::GetDataPtr( const char* scope_data, int index ) const
{
  return scope_data ? scope_data + m_Offset + index * GetSize() : NULL;
}

sbScalarValue sbMember::ReadValue( const char* scope_data ) const
{
  return m_Type->ReadValue( scope_data + m_Offset );
}

size_t sbMember::GetTotalSize() const
{
  return GetSize() * m_Count;
}

sbStatus sbMember::FixUp( sbSchema* schema, const sbMember* previous_member )
{
  sbStatus status = PreFixUp( schema, m_TypeName );
  
  if( status == sbStatus_Ok )
  {
    m_Type = schema->FindType( m_TypeName );
    if( !m_Type )
    {
      status = sbStatus_ErrorNodeNotFound;
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

sbMember* sbMember::Read( sbByteReader* reader, const sbAggregateType* scope )
{
  sbMember* member = NULL;
  if( !member ) member = sbInstanceMember::Read( reader, scope );
  if( !member ) member = sbPointerMember::Read( reader, scope );
  if( !member ) member = sbCountedPointerMember::Read( reader, scope );
  if( !member ) member = sbStringPointerMember::Read( reader, scope );
  return member;
}
