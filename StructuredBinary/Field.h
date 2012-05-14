//
//  Field.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_Field_h
#define StructuredBinary_Field_h

// Libraries
#include <cstring>
#include <stdint.h>

// Project
#include "Number.h"
#include "Fnv.h"

class Aggregate;

class Field
{
public:
  virtual Number ReadNumber( const char* data ) const = 0;  
private:
};

class FieldFloat64 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Float( *( double* )data );
  }
};

class FieldFloat32 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Float( *( float* )data );
  }
};

class FieldInt64 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Int( *( int64_t* )data );
  }
};

class FieldUInt64 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::UInt( *( uint64_t* )data );
  }
};

class FieldInt32 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Int( *( int32_t* )data );
  }
};

class FieldUInt32 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::UInt( *( uint32_t* )data );
  }
};

class FieldInt16 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Int( *( int16_t* )data );
  }
};

class FieldUInt16 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::UInt( *( uint16_t* )data );
  }
};

class FieldInt8 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Int( *( int8_t* )data );
  }
};

class FieldUInt8 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::UInt( *( uint8_t* )data );
  }
};

#endif
