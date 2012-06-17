//
//  sbValue.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbValue_h
#define StructuredBinary_sbValue_h

// Libraries
#include <stdint.h>

class sbValue
{
public:
  
  sbValue();
  
  static sbValue ReinterpretFloatFromU32( uint32_t i );
  static sbValue Float( double value );
  static sbValue Int( int64_t value );
  static sbValue Null();
  
  double AsFloat() const;
  int64_t AsInt() const;
  
  bool operator== ( const sbValue& other ) const;
  bool operator!= ( const sbValue& other ) const;
  
  bool IsFloat() const  { return m_Type == kFloat; }
  bool IsInt() const    { return m_Type == kInt; }
  bool IsNull() const   { return m_Type == kNull; }
  
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
