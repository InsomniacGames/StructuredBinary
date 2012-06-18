//
//  sbAggregateType.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//


#include "sbAggregateType.h"

#include <stdio.h>
#include <assert.h>
#include <cstring>

#include "sbUtil.h"
#include "sbSchema.h"
#include "sbScalarType.h"
#include "sbValue.h"
#include "sbStatus.h"

#include "sbInstanceMember.h"
#include "sbStringPointerMember.h"
#include "sbCountedPointerMember.h"

const sbMember* sbAggregateType::FindMember( sbHash member_name ) const
{
  return m_Dictionary.FindByName( member_name );
}

sbStatus sbAggregateType::Convert( char* dst_data, const char* src_data, const sbType* src_type, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    const sbMember* dst_member = m_Dictionary.GetByIndex( i );
    sbHash dst_member_name = m_Dictionary.GetNameByIndex( i );
    const sbMember* src_member = src_type->FindMember( dst_member_name );
    dst_member->Convert( dst_data, src_data, src_member, alloc );
  }
  return status;
}

bool sbAggregateType::IsTerminal( const char* data, const sbValue& terminator_value, sbHash terminator_name ) const
{
  const sbMember* member = FindMember( terminator_name );
  assert( member );
  return member->ReadValue( data ) == terminator_value;
}

sbStatus sbAggregateType::FixUp( sbSchema* schema )
{
  sbStatus status = sbStatus_Ok;

  if( m_State == kState_Ready ) return status;
  assert( m_State == kState_Defined );
  
  m_State = kState_Fixing;

  size_t alignment = 1;
  const sbMember* previous_member = NULL;
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    sbMember* member = m_Dictionary.GetByIndex( i );
    status = member->FixUp( schema, previous_member );

    if( status != sbStatus_Ok )
    {
      break;
    }

    size_t member_alignment = 0;
    member_alignment = member->GetAlignment();
    alignment = member_alignment > alignment ? member_alignment : alignment;

    previous_member = member;
  }

  size_t size = previous_member->GetOffset() + previous_member->GetTotalSize();

  m_Size = FIX_ALIGNMENT( size, alignment );
  m_Alignment = alignment;

  m_State = kState_Ready;

  return status;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------

void sbAggregateType::AddMember( sbHash member_name, sbMember* member )
{
  m_Dictionary.Add( member_name, member );
}

void sbAggregateType::AddInstance( sbHash member_name, int count, sbHash type_name )
{
  AddMember( member_name, new sbInstanceMember( this, count, type_name ) );
}

void sbAggregateType::AddPointer( sbHash member_name, int count, sbHash type_name, sbHash count_name )
{
  AddMember( member_name, new sbCountedPointerMember( this, count, type_name, count_name ) );
}

void sbAggregateType::AddString( sbHash member_name, int count, sbHash type_name, sbHash terminator_name, const sbValue& terminator_value )
{
  AddMember( member_name, new sbStringPointerMember( this, count, type_name, terminator_name, terminator_value ) );
}

sbAggregateType::~sbAggregateType()
{
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    delete m_Dictionary.GetByIndex( i );
  }
}

sbValue sbAggregateType::ReadValue( const char* data ) const
{
  assert( false );  
}
