//
//  Aggregate.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_Aggregate_h
#define StructuredBinary_Aggregate_h

// Libraries
#include <cstring>
#include <stdint.h>

// Project
#include "Number.h"
#include "Fnv.h"
#include "Field.h"

class Aggregate
{
public:
  Aggregate( int field_count )
  : m_EntryCount( field_count )
  , m_Entry( new Entry[ field_count ] )
  {}

  ~Aggregate()
  {
    delete[] m_Entry;
  }

  void SetField( int index, int offset, const char* name, const Field* field )
  {
    SetField( index, offset, Fnv32( name ), field );
  }

  void SetField( int index, int offset, uint32_t name, const Field* field )
  {
    m_Entry[ index ].m_Offset = offset;
    m_Entry[ index ].m_Name = name;
    m_Entry[ index ].m_Field = field;
  }
  
  int GetFieldCount() const { return m_EntryCount; }

  Number Read( const char* data, const char* name ) const
  {
    return Read( data, Fnv32( name ) );
  }

  Number Read( const char* data, uint32_t name ) const
  {
    for( int i = 0; i < m_EntryCount; ++i )
    {
      if( m_Entry[ i ].m_Name == name )
        return m_Entry[ i ].m_Field->ReadNumber( data + m_Entry[ i ].m_Offset );
    }
    return Number::Null();
  }
  
  void Convert( char* data, const char* source_data, const Aggregate* source_agg ) const;

private:
  struct Entry
  {
    Entry()
    : m_Name( 0 )
    {}
    uint32_t      m_Name;
    int           m_Offset;
    const Field*  m_Field;
  };

  int     m_EntryCount;
  Entry*  m_Entry;
};

#endif
