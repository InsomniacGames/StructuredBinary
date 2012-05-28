//
//  sbSchema.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/27/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include <assert.h>

#include "sbSchema.h"

sbSchema::~sbSchema()
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    delete m_Entries[ i ].m_Struct;
  }
  delete[] m_Entries;
}

void sbSchema::AddStruct( uint32_t name, const sbStruct* str )
{
  assert( m_EntryCount < m_EntryMax );
  int index = m_EntryCount++;

  m_Entries[ index ].m_Name = name;
  m_Entries[ index ].m_Struct = str;
}

const sbStruct* sbSchema::FindStruct( uint32_t name ) const
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    const Entry* entry = m_Entries + i;
    if( entry->m_Name == name )
    {
      return entry->m_Struct;
    }
  }
  return NULL;
}


