//
//  sbPointerField.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbPointerField_h
#define StructuredBinary_sbPointerField_h

#include <stddef.h>

#include "sbField.h"
#include "sbHash.h"

class sbAggregate;
class sbAllocator;

class sbPointerField : public sbField
{  
public:
  sbPointerField( const sbAggregate* parent, sbHash field_name, int count, sbHash element_name );
  virtual size_t GetSize() const;
  virtual size_t GetAlignment() const;
  virtual void Convert( char* dst_aggregate_data, const char* src_aggregate_data, const sbField* src_field, sbAllocator* alloc ) const;
  virtual int GetPointerCount( const char* aggregate_data, int index ) const;
};

#endif
