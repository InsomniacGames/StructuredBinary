//
//  sbStringPointerField.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbStringPointerField_h
#define StructuredBinary_sbStringPointerField_h

#include "sbPointerField.h"

#include "sbValue.h"
#include "sbHash.h"

class sbAggregate;

class sbStringPointerField : public sbPointerField
{
  sbValue m_TerminatorValue;
  sbHash   m_TerminatorName;
  
public:
  sbStringPointerField( const sbAggregate* aggregate, int count, sbHash element_name, sbHash terminator_name, const sbValue& terminator_value );
  virtual int GetPointerCount( const char* aggregate_data, int index ) const;
};




#endif
