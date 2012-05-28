//
//  sbSchema.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/27/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbSchema_h
#define StructuredBinary_sbSchema_h

// Libraries
#include <cstring>
#include <stdint.h>
// Project
#include "sbStruct.h"

class sbSchema
{
  static const int s_InitialEntryMax = 50;

public:

  sbSchema()
  : m_EntryMax( s_InitialEntryMax )
  , m_EntryCount( 0 )
  , m_Entries( new Entry[ s_InitialEntryMax ] )
  {}
  ~sbSchema();

  void AddStruct( uint32_t name, const sbStruct* str );
  const sbStruct* FindStruct( uint32_t name ) const;

private:
  struct Entry
  {
    uint32_t        m_Name;
    const sbStruct* m_Struct;
  };
  
  int     m_EntryMax;
  int     m_EntryCount;
  Entry*  m_Entries;
};

#endif
