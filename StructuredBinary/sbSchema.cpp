//
//  sbSchema.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbSchema.h"

#include <cstring>
#include <assert.h>

#include "sbUtil.h"
#include "sbAggregateType.h"
#include "sbScalarValue.h"
#include "sbStatus.h"

#include "sbFloatScalarType.h"
#include "sbIntScalarType.h"

#include "sbDictionary.h"
#include "sbByteWriter.h"
#include "sbByteReader.h"
#include "sbHash.h"

const sbType* sbSchema::FindType( sbHash name ) const
{
  return m_Dictionary.FindByName( name );
}

sbType* sbSchema::FindType( sbHash name )
{
  return m_Dictionary.FindByName( name );
}

sbStatus sbSchema::FixUp()
{
  sbStatus status = sbStatus_Ok;
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    status = m_Dictionary.GetByIndex( i )->FixUp( this );
    if( status != sbStatus_Ok )
      break;
  }
  return status;
}

sbStatus sbSchema::FixUp( sbHash type_name )
{
  sbStatus status = sbStatus_Ok;
  sbType* type_def = m_Dictionary.FindByName( type_name );
  if( !type_name )
  {
    status = sbStatus_ErrorNodeNotFound;
  }
  else
  {
    status = type_def->FixUp( this );
  }
  return status;
}

sbStatus sbSchema::Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, sbHash name, sbAllocator* alloc ) const
{
  sbStatus status = sbStatus_Ok;

  const sbType* src_type = src_schema->FindType( name );
  const sbType* dst_type =             FindType( name );
  
  if( !src_type || !dst_type )
  {
    status = sbStatus_ErrorNodeNotFound;
  }
  else
  {
    status = dst_type->Convert( dst_data, src_data, src_type, alloc );
  }
  return status;
}

void sbSchema::BeginAggregate( sbHash type_name )
{
  assert( !m_CurrentAggregate );
  
  m_CurrentName = type_name;
  m_CurrentAggregate = new sbAggregateType();
}

void sbSchema::EndAggregate()
{
  AddType( m_CurrentName, m_CurrentAggregate );
  m_CurrentName = 0U;
  m_CurrentAggregate = NULL;
}

void sbSchema::AddInstance( sbHash member_name, int count, sbHash type_name )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddInstance( member_name, count, type_name );
}

void sbSchema::AddPointer( sbHash member_name, int count, sbHash type_name )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddPointer( member_name, count, type_name );
}

void sbSchema::AddCountedPointer( sbHash member_name, int count, sbHash type_name, sbHash count_name )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddCountedPointer( member_name, count, type_name, count_name );
}

void sbSchema::AddStringPointer( sbHash member_name, int count, sbHash type_name, sbHash terminator_name, const sbScalarValue& terminator_value )
{
  assert( m_CurrentAggregate );
  m_CurrentAggregate->AddStringPointer( member_name, count, type_name, terminator_name, terminator_value );
}

void sbSchema::AddType( sbHash type_name, sbType* sb_type )
{
  m_Dictionary.Add( type_name, sb_type );
}

void sbSchema::Begin()
{
  assert( m_State == kState_New );
  m_State = kState_Building;

  AddType( "uint8_t",   new sbIntScalarType<  uint8_t > );
  AddType( "int8_t",    new sbIntScalarType<   int8_t > );
  AddType( "uint16_t",  new sbIntScalarType< uint16_t > );
  AddType( "int16_t",   new sbIntScalarType<  int16_t > );
  AddType( "uint32_t",  new sbIntScalarType< uint32_t > );
  AddType( "int32_t",   new sbIntScalarType<  int32_t > );
  AddType( "uint64_t",  new sbIntScalarType< uint64_t > );
  AddType( "int64_t",   new sbIntScalarType<  int64_t > );
  AddType( "float",     new sbFloatScalarType<  float > );
  AddType( "double",    new sbFloatScalarType< double > );
}

void sbSchema::End()
{
  assert( m_State == kState_Building );
  FixUp();
  m_State = kState_Ready;
}

sbSchema::~sbSchema()
{
  for( int i = 0; i < m_Dictionary.GetCount(); ++i )
  {
    delete m_Dictionary.GetByIndex( i );
  }
}

sbSchema::sbSchema()
: m_State( kState_New )
, m_CurrentAggregate( NULL)
, m_Dictionary( 16, NULL )
{}

void sbSchema::Write( sbByteWriter* writer ) const
{
  int count = m_Dictionary.GetCount();

  int serialized_count = count;
  for( int i = 0; i < count; ++i )
  {
    const sbType* t = m_Dictionary.GetByIndex( i );
    if( t->IsBuiltIn() ) // Skip built-in types, such as uint8_t
    {
      serialized_count -= 1;
    }
  }
  
  writer->Write16( serialized_count );

  for( int i = 0; i < count; ++i )
  {
    const sbType* t = m_Dictionary.GetByIndex( i );
    if( !t->IsBuiltIn() ) // Skip built-in types, such as uint8_t
    {
      sbHash name = m_Dictionary.GetNameByIndex( i );
      writer->Write32( name );
      writer->Write( t );
    }
  }
}

sbSchema* sbSchema::ReadNew( sbByteReader* reader )
{
  size_t roll_back = reader->Tell();
  sbSchema* schema = new sbSchema();

  int count = reader->Read16();
  schema->Begin();

  for( int i = 0; i < count; ++i )
  {
    sbHash type_name = reader->Read32();
    sbType* sb_type = reader->ReadNew< sbType >();
    if( sb_type )
    {
      schema->AddType( type_name, sb_type );
    }
    else
    {
      delete schema;
      schema = NULL;
      break;
    }
  }

  if( !schema )
  {
    reader->Seek( roll_back );
  }
  else
  {
    schema->End();
  }
  return schema;
}

uint64_t sbSchema::GetChecksum( uint64_t basis ) const
{
  int count = m_Dictionary.GetCount();
  for( int i = 0; i < count; ++i )
  {
    const sbType* t = m_Dictionary.GetByIndex( i );
    if( !t->IsBuiltIn() ) // Skip built-in types, such as uint8_t
    {
      sbHash name = m_Dictionary.GetNameByIndex( i );
      basis = sbFnv64( basis, ( uint32_t )name );
      basis = t->GetChecksum( basis );
    }
  }
  return basis;
}
