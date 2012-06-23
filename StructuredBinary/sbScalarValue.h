//
//  sbScalarValue.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbScalarValue_h
#define StructuredBinary_sbScalarValue_h

// Libraries
#include <stdint.h>

class sbByteReader;
class sbByteWriter;

// TO DO: Can I add support for fixed-point values and booleans, right here in sbScalarValue?

class sbScalarValue
{
public:
  
  sbScalarValue();
  
  static sbScalarValue ReinterpretFloatFromU32( uint32_t i );
  static sbScalarValue Float( double value );
  static sbScalarValue Int( int64_t value );
  static sbScalarValue Null();
  
  double AsFloat() const;
  int64_t AsInt() const;
  
  bool operator== ( const sbScalarValue& other ) const;
  bool operator!= ( const sbScalarValue& other ) const;
  
  bool IsFloat() const  { return m_Type == kFloat; }
  bool IsInt() const    { return m_Type == kInt; }
  bool IsNull() const   { return m_Type == kNull; }

  void Write( sbByteWriter* writer ) const;
  static sbScalarValue Read( sbByteReader* reader );

private:
  
  enum Type
  {
    kNull,
    kInt,
    kFloat,
  };

  Type    m_Type;
  
  union
  {
    double    f64;
    int64_t   i64;
  } m_Value;
};

#endif
