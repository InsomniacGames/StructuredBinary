//
//  sbScalar.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbScalar_h
#define StructuredBinary_sbScalar_h

// Libraries
#include <cstring>
#include <stdint.h>

// Project
#include "sbScalarValue.h"
#include "sbFnv.h"

class sbScalar
{
public:
  virtual ~sbScalar() {}
  virtual sbScalarValue ReadValue( const char* data ) const { return sbScalarValue::Null(); }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const {}

  virtual void Convert( char* write_data, const char* read_data, const sbScalar* read_scalar ) const
  {
    sbScalarValue value = read_scalar->ReadValue( read_data );
    WriteValue( write_data, value );
  }

  virtual int GetSize() const = 0;
  virtual int GetAlign() const = 0;
};

class sbScalarF64 : public sbScalar
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::Float( *( double* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
  {
    *( double* )data = ( double )number.AsFloat();
  }
  virtual int GetSize() const { return sizeof( double ); }
  virtual int GetAlign() const { return ( int )__alignof( double ); };
};

class sbScalarF32 : public sbScalar
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::Float( *( float* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
  {
    *( float* )data = ( float )number.AsFloat();
  }
  virtual int GetSize() const { return sizeof( float ); }
  virtual int GetAlign() const { return ( int )__alignof( float ); };
};

class sbScalarI64 : public sbScalar
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::Int( *( int64_t* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
  {
    *( int64_t* )data = ( int64_t )number.AsInt();
  }
  virtual int GetSize() const { return sizeof( int64_t ); }
  virtual int GetAlign() const { return ( int )__alignof( int64_t ); };
};

class sbScalarU64 : public sbScalar
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::UInt( *( uint64_t* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
  {
    *( uint64_t* )data = ( uint64_t )number.AsInt();
  }
  virtual int GetSize() const { return sizeof( uint64_t ); }
  virtual int GetAlign() const { return ( int )__alignof( uint64_t ); };
};

class sbScalarI32 : public sbScalar
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::Int( *( int32_t* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
  {
    *( int32_t* )data = ( int32_t )number.AsInt();
  }
  virtual int GetSize() const { return sizeof( int32_t ); }
  virtual int GetAlign() const { return ( int )__alignof( int32_t ); };
};

class sbScalarU32 : public sbScalar
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::UInt( *( uint32_t* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
  {
    *( uint32_t* )data = ( uint32_t )number.AsInt();
  }
  virtual int GetSize() const { return sizeof( uint32_t ); }
  virtual int GetAlign() const { return ( int )__alignof( uint32_t ); };
};

class sbScalarI16 : public sbScalar
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::Int( *( int16_t* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
  {
    *( int16_t* )data = ( int16_t )number.AsInt();
  }
  virtual int GetSize() const { return sizeof( int16_t ); }
  virtual int GetAlign() const { return ( int )__alignof( int16_t ); };
};

class sbScalarU16 : public sbScalar
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::UInt( *( uint16_t* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
  {
    *( uint16_t* )data = ( uint16_t )number.AsInt();
  }
  virtual int GetSize() const { return sizeof( uint16_t ); }
  virtual int GetAlign() const { return ( int )__alignof( uint16_t ); };
};

class sbScalarI8 : public sbScalar
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::Int( *( int8_t* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
  {
    *( int8_t* )data = ( int8_t )number.AsInt();
  }
  virtual int GetSize() const { return sizeof( int8_t ); }
  virtual int GetAlign() const { return ( int )__alignof( int8_t ); };
};

class sbScalarU8 : public sbScalar
{
public:
  virtual sbScalarValue ReadValue( const char* data ) const
  {
    return sbScalarValue::UInt( *( uint8_t* )data );
  }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const
  {
    *( uint8_t* )data = ( uint8_t )number.AsInt();
  }
  virtual int GetSize() const { return sizeof( uint8_t ); }
  virtual int GetAlign() const { return ( int )__alignof( uint8_t ); };
};


#endif
