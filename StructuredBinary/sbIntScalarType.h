//
//  sbIntScalarType.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbIntScalarType_h
#define StructuredBinary_sbIntScalarType_h

#include "sbScalarType.h"

template< typename T >
class sbIntScalarType : public sbScalarType
{
public:
  virtual sbScalarValue ReadScalarValue( const char* data ) const
  {
    return sbScalarValue::Int( *( T* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& value ) const
  {
    *( T* )data = ( T )value.AsInt();
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
  virtual void Write( sbByteWriter* writer ) const {} // All integer scalar types are built-in
};

#endif
