//
//  sbFloatScalarType.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbFloatScalarType_h
#define StructuredBinary_sbFloatScalarType_h

#include "sbScalarType.h"

template< typename T >
class sbFloatScalarType : public sbScalarType
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::Float( *( T* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
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

  virtual bool IsBuiltIn() const { return true; }
  virtual void Write( sbByteWriter* writer ) const
  {} // All float scalar types are built-in
};

#endif
