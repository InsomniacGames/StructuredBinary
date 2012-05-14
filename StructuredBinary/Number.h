//
//  Number.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_Number_h
#define StructuredBinary_Number_h

// Libraries
#include <stdint.h>

class Number
{
public:
  
  Number();
  
  static Number ReinterpretFloatFromU32( uint32_t i );
  static Number Float( double value );
  static Number Int( int64_t value );
  static Number UInt( uint64_t value );
  static Number Null();
  
  double AsFloat() const;
  int64_t AsInt() const;
  
  bool operator== ( const Number& other ) const;
  bool operator!= ( const Number& other ) const;
  
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
