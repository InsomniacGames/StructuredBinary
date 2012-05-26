//
//  sbWriteCursor.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/26/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbWriteCursor_h
#define StructuredBinary_sbWriteCursor_h

#include "stdint.h"

class sbField;

struct WriteCursor
{
  WriteCursor( char* data, uint32_t name, const sbField* field )
  : m_Name( name )
  , m_Data( data )
  , m_Field( field )
  {}
  uint32_t      m_Name; // Remove me
  char*         m_Data;
  const sbField*  m_Field;
};


#endif
