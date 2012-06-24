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
#include "sbScalarValue.h"
#include "sbStatus.h"

#include "sbInstanceMember.h"
#include "sbPointerMember.h"
#include "sbCountedPointerMember.h"
#include "sbStringPointerMember.h"

#include "sbByteWriter.h"
#include "sbByteReader.h"

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

bool sbAggregateType::IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const
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
  member->SetScope( this );
  m_Dictionary.Add( member_name, member );
}

void sbAggregateType::AddInstance( sbHash member_name, int count, sbHash type_name )
{
  AddMember( member_name, new sbInstanceMember( count, type_name ) );
}

void sbAggregateType::AddPointer( sbHash member_name, int count, sbHash type_name )
{
  AddCountedPointer( member_name, count, type_name, 0U );
}

void sbAggregateType::AddCountedPointer( sbHash member_name, int count, sbHash type_name, sbHash count_name )
{
  AddMember( member_name, new sbCountedPointerMember( count, type_name, count_name ) );
}

void sbAggregateType::AddStringPointer( sbHash member_name, int count, sbHash type_name, sbHash terminator_name, const sbScalarValue& terminator_value )
{
  AddMember( member_name, new sbStringPointerMember( count, type_name, terminator_name, terminator_value ) );
}

sbAggregateType::~sbAggregateType()
{
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    delete m_Dictionary.GetByIndex( i );
  }
}

sbScalarValue sbAggregateType::ReadValue( const char* data ) const
{
  assert( false );  
}

void sbAggregateType::Write( sbByteWriter* writer ) const
{
  writer->Write8( ByteCode_Aggregate );

  int count = m_Dictionary.GetCount();
  writer->Write16( count );
  
  for( int i = 0; i < count; ++i )
  {
    const sbMember* member = m_Dictionary.GetByIndex( i );
    sbHash member_name = m_Dictionary.GetNameByIndex( i );
    writer->Write32( member_name );
    writer->Write( member );
  }
}

sbAggregateType* sbAggregateType::ReadNew( sbByteReader* reader )
{
  size_t roll_back = reader->Tell();
  sbAggregateType* aggregate = NULL;
  
  ByteCode code = ( ByteCode )reader->Read8();

  switch( code )
  {
    case ByteCode_Aggregate:
    {
      aggregate = new sbAggregateType();
      int count = reader->Read16();
      
      for( int i = 0; i < count; ++i )
      {
        sbHash member_name = reader->Read32();
        sbMember* member = reader->ReadNew< sbMember >();
        
        if( member )
        {
          aggregate->AddMember( member_name, member );
        }
        else
        {
          // Could not read member. Bail out.
          delete aggregate;
          aggregate = NULL;
          break;
        }
      }
    }
    default:
      break;
  }

  if( !aggregate )
  {
    reader->Seek( roll_back );
  }
  return aggregate;
}

uint64_t sbAggregateType::GetChecksum( uint64_t basis ) const
{
  int count = m_Dictionary.GetCount();
  for( int i = 0; i < count; ++i )
  {
    const sbMember* member = m_Dictionary.GetByIndex( i );
    sbHash name = m_Dictionary.GetNameByIndex( i );
    basis = sbFnv64( basis, ( uint32_t )name );
    basis = member->GetChecksum( basis );
  }
  return basis;
}

