//
//  sbElementField.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbElementField_h
#define StructuredBinary_sbElementField_h

#include "sbField.h"
#include "sbHash.h"

class sbAggregate;
class Allocator;

class sbElementField : public sbField
{
public:
  sbElementField( const sbAggregate* aggregate, int count, sbHash element_name );
  virtual size_t GetSize() const;
  virtual size_t GetAlignment() const;
  virtual int GetPointerCount( const char* aggregate_data, int index ) const;
  virtual void Convert( char* dst_data, const char* src_data, const sbField* src_child, sbAllocator* alloc ) const;
  virtual sbStatus FixUp( sbSchema* schema, const sbField* previous_field );
};


#endif
