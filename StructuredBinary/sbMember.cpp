//
//  sbMember.cpp.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbMember.h"

#include "sbType.h"
#include "sbValue.h"
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

sbValue sbMember::ReadValue( const char* scope_data ) const
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

sbMember* sbMember::Read( const sbAggregateType* scope, sbByteReader* reader )
{
  sbMember* member;
  sbMemberType member_type = ( sbMemberType )reader->Read8();
  switch( member_type )
  {
    case sbMemberType_Instance:
    {
      sbHash type_name        = reader->Read32();
      int count               = 1;
      member = new sbInstanceMember( scope, count, type_name );
      break;
    }
    case sbMemberType_InstanceArray:
    {
      sbHash type_name        = reader->Read32();
      int count               = reader->Read16();
      member = new sbInstanceMember( scope, count, type_name );
      break;
    }
    case sbMemberType_Pointer:
    {
      sbHash type_name        = reader->Read32();
      int count               = 1;
      member = new sbPointerMember( scope, count, type_name );
      break;
    }
    case sbMemberType_PointerArray:
    {
      sbHash type_name        = reader->Read32();
      int count               = reader->Read16();
      member = new sbPointerMember( scope, count, type_name );
      break;
    }
    case sbMemberType_CountedPointer:
    {
      sbHash type_name        = reader->Read32();
      int count               = 1;
      sbHash count_name       = reader->Read32();
      member = new sbCountedPointerMember( scope, count, type_name, count_name );
      break;
    }
    case sbMemberType_CountedPointerArray:
    {
      sbHash type_name        = reader->Read32();
      int count               = reader->Read16();
      sbHash count_name       = reader->Read32();
      member = new sbCountedPointerMember( scope, count, type_name, count_name );
      break;
    }
    case sbMemberType_StringPointer:
    {
      sbHash type_name          = reader->Read32();
      int count                 = 1;
      sbHash terminator_name    = reader->Read32();
      sbValue terminator_value  = sbValue::Read( reader );
      member = new sbStringPointerMember( scope, count, type_name, terminator_name, terminator_value );
      break;
    }
    case sbMemberType_StringPointerArray:
    {
      sbHash type_name        = reader->Read32();
      int count               = reader->Read16();
      sbHash terminator_name  = reader->Read32();
      sbValue terminator_value  = sbValue::Read( reader );
      member = new sbStringPointerMember( scope, count, type_name, terminator_name, terminator_value );
      break;
    }
  }
  return member;
}
