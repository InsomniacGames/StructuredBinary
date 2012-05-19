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

// This was made at work

class Field;

struct WriteCursor
{
  WriteCursor( char* data, uint32_t name, const Field* field )
  : m_Name( name )
  , m_Data( data )
  , m_Field( field )
  {}
  uint32_t      m_Name; // Remove me
  char*         m_Data;
  const Field*  m_Field;
};

struct ReadCursor
{
  ReadCursor( const char* data, uint32_t name, const Field* field )
  : m_Name( name )
  , m_Data( data )
  , m_Field( field )
  {}
  uint32_t      m_Name; // Remove me
  const char*   m_Data;
  const Field*  m_Field;
};

class Field
{
public:
  virtual Number ReadNumber( const char* data ) const { return Number::Null(); }
  virtual void WriteNumber( char* data, const Number& number ) const {}

  virtual void Convert( char* dst_data, const ReadCursor& rc ) const
  {
    Number n = rc.m_Field->ReadNumber( rc.m_Data );
    WriteNumber( dst_data, n );
  }

  virtual ReadCursor Find( const char* data, uint32_t name ) const
  {
    return ReadCursor( NULL, 0, NULL );
  }

  virtual WriteCursor Find( char* data, uint32_t name ) const
  {
    return WriteCursor( NULL, 0, NULL );
  }

  virtual void FixSizeAndStride() {}
  virtual int GetElementSize() const = 0;
  virtual int GetElementStride() const { return GetElementSize(); }

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
  virtual int GetElementSize() const { return 8; }
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
  virtual int GetElementSize() const { return 4; }
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
  virtual int GetElementSize() const { return 8; }
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
  virtual int GetElementSize() const { return 8; }
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
  virtual int GetElementSize() const { return 4; }
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
  virtual int GetElementSize() const { return 4; }
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
  virtual int GetElementSize() const { return 2; }
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
  virtual int GetElementSize() const { return 2; }
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
  virtual int GetElementSize() const { return 1; }
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
  virtual int GetElementSize() const { return 1; }
};

#endif
