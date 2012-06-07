//
//  sbPath.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbPath_h
#define StructuredBinary_sbPath_h

#include <cstring>

struct sbPath
{
  sbPath( const sbPath* parent )
  : m_Parent( parent )
  , m_Name( NULL )
  {}
  const sbPath* m_Parent;
  const char*   m_Name;
};

#endif
