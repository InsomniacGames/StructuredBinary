//
//  Aggregate.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "Aggregate.h"
// Libraries
#include <cstring>
#include <stdint.h>
// Project
#include "Number.h"
#include "Fnv.h"
#include "Field.h"

void Aggregate::Convert( char* data, const char* source_data, const Aggregate* source_agg ) const
{
  for( int i = 0; i < m_EntryCount; ++i )
  {
    Number n = source_agg->Read( source_data, m_Entry[ i ].m_Name );
    m_Entry[ i ].m_Field->WriteNumber( data + m_Entry[ i ].m_Offset, n );
  }
}
