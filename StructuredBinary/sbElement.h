//
//  sbElement.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbElement_h
#define StructuredBinary_sbElement_h

#include <stdint.h>

#include "sbStatus.h"
#include "sbHash.h"

class sbValue;

class sbElement
{
public:
  virtual ~sbElement() {}
  
  virtual sbStatus Convert( char* dst_element_data, const char* src_element_data, const sbElement* src_node, class sbAllocator* alloc ) const = 0;
  virtual size_t GetSize() const = 0;
  virtual size_t GetAlignment() const = 0;
  
  virtual bool IsTerminal( const char* element_data, const sbValue& terminator_value, sbHash terminator_name ) const = 0;
  
  virtual sbStatus FixUp( class sbSchema* schema ) = 0;
  virtual const class sbField* FindField( sbHash name ) const = 0;
  
  virtual sbValue ReadValue( const char* data ) const = 0;
};

#endif
