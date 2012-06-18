//
//  sbIntType.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbIntType_h
#define StructuredBinary_sbIntType_h

#include "sbScalarType.h"

template< typename T >
class sbIntType : public sbScalarType
{
public:
  virtual sbValue ReadValue( const char* data ) const
  {
    return sbValue::Int( *( T* )data );
  }
  virtual void WriteValue( char* data, const sbValue& number ) const
  {
    *( T* )data = ( T )number.AsInt();
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
