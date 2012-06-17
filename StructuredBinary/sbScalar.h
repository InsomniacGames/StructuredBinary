//
//  sbScalar.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbScalar_h
#define StructuredBinary_sbScalar_h

// Libraries
#include <cstring>
#include <stdint.h>
#include <stdio.h>

// Project
#include "sbValue.h"
#include "sbHash.h"
#include "sbStatus.h"
#include "sbElement.h"
#include "sbField.h"
#include "sbAllocator.h"

class sbScalar : public sbElement
{
public:
  virtual ~sbScalar();
  virtual void WriteValue( char* data, const sbValue& number ) const = 0;
  virtual sbStatus Convert( char* dst_element_data, const char* src_element_data, const sbElement* src_element, class sbAllocator* alloc ) const;

  virtual bool IsTerminal( const char* data, const sbValue& terminator_value, sbHash terminator_name ) const;
  virtual sbStatus FixUp( class sbSchema* schema );
  virtual const sbField* FindField( sbHash name ) const;
};

#endif
