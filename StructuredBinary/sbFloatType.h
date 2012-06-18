//
//  sbFloatType.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbFloatType_h
#define StructuredBinary_sbFloatType_h

#include "sbScalarType.h"

template< typename T >
class sbFloatType : public sbScalarType
{
public:
  virtual sbValue ReadValue( const char* data ) const
  {
    return sbValue::Float( *( T* )data );
  }
  virtual void WriteValue( char* data, const sbValue& number ) const
  {
    *( T* )data = ( T )number.AsFloat();
  }
  virtual size_t GetSize() const
  {
    return sizeof( T );
  }
  virtual size_t GetAlignment() const
  {
    return __alignof( T );
  }
};

#endif
