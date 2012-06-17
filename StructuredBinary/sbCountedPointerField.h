//
//  sbCountedPointerField.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbCountedPointerField_h
#define StructuredBinary_sbCountedPointerField_h

#include "sbPointerField.h"

class sbCountedPointerField : public sbPointerField
{
  sbHash   m_CountName;
  
public:
  sbCountedPointerField( const sbAggregate* parent, int count, sbHash element_name, sbHash count_name );
  virtual int GetPointerCount( const char* parent_data, int index ) const;
};


#endif
