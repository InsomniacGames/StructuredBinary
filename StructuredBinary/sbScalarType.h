//
//  sbScalarType.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbScalarType_h
#define StructuredBinary_sbScalarType_h

// Libraries

// Project
#include "sbScalarValue.h"
#include "sbHash.h"
#include "sbStatus.h"
#include "sbType.h"
#include "sbMember.h"
#include "sbAllocator.h"

class sbScalarType : public sbType
{
public:
  virtual ~sbScalarType();
  virtual void WriteValue( char* data, const sbScalarValue& value ) const = 0;
  virtual sbStatus ConvertOne( char* dst_data, const char* src_data, const sbType* src_type, class sbAllocator* alloc ) const;

  virtual bool IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const;
  virtual sbStatus FixUp( class sbSchema* schema );

  virtual uint64_t GetChecksum( uint64_t basis ) const { return basis; }
};

#endif
