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
  virtual void WriteNumber( char* data, const Number& number ) const = 0;
private:
};

class FieldFloat64 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Float( *( double* )data );
  }
  virtual void WriteNumber( char* data, const Number& number ) const
  {
    *( double* )data = ( double )number.AsFloat();
  }
};

class FieldFloat32 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Float( *( float* )data );
  }
  virtual void WriteNumber( char* data, const Number& number ) const
  {
    *( float* )data = ( float )number.AsFloat();
  }
};

class FieldInt64 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Int( *( int64_t* )data );
  }
  virtual void WriteNumber( char* data, const Number& number ) const
  {
    *( int64_t* )data = ( int64_t )number.AsInt();
  }
};

class FieldUInt64 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::UInt( *( uint64_t* )data );
  }
  virtual void WriteNumber( char* data, const Number& number ) const
  {
    *( uint64_t* )data = ( uint64_t )number.AsInt();
  }
};

class FieldInt32 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Int( *( int32_t* )data );
  }
  virtual void WriteNumber( char* data, const Number& number ) const
  {
    *( int32_t* )data = ( int32_t )number.AsInt();
  }
};

class FieldUInt32 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::UInt( *( uint32_t* )data );
  }
  virtual void WriteNumber( char* data, const Number& number ) const
  {
    *( uint32_t* )data = ( uint32_t )number.AsInt();
  }
};

class FieldInt16 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Int( *( int16_t* )data );
  }
  virtual void WriteNumber( char* data, const Number& number ) const
  {
    *( int16_t* )data = ( int16_t )number.AsInt();
  }
};

class FieldUInt16 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::UInt( *( uint16_t* )data );
  }
  virtual void WriteNumber( char* data, const Number& number ) const
  {
    *( uint16_t* )data = ( uint16_t )number.AsInt();
  }
};

class FieldInt8 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::Int( *( int8_t* )data );
  }
  virtual void WriteNumber( char* data, const Number& number ) const
  {
    *( int8_t* )data = ( int8_t )number.AsInt();
  }
};

class FieldUInt8 : public Field
{
public:
  virtual Number ReadNumber( const char* data ) const
  {
    return Number::UInt( *( uint8_t* )data );
  }
  virtual void WriteNumber( char* data, const Number& number ) const
  {
    *( uint8_t* )data = ( uint8_t )number.AsInt();
  }
};

#endif
