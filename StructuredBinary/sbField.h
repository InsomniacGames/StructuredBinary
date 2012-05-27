//
//  sbField.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbField_h
#define StructuredBinary_sbField_h

// Libraries
#include <cstring>
#include <stdint.h>

// Project
#include "sbNumber.h"
#include "sbFnv.h"

class sbField
{
public:
  virtual ~sbField() {}
  virtual sbNumber ReadNumber( const char* data ) const { return sbNumber::Null(); }
  virtual void WriteNumber( char* data, const sbNumber& number ) const {}

  virtual void Convert( char* write_data, const char* read_data, const sbField* read_field ) const
  {
    sbNumber n = read_field->ReadNumber( read_data );
    WriteNumber( write_data, n );
  }

  virtual int GetElementSize() const = 0;
  virtual int GetElementAlign() const = 0;
};

class sbFieldF64 : public sbField
{
public:
  virtual sbNumber ReadNumber( const char* data ) const
  {
    return sbNumber::Float( *( double* )data );
  }
  virtual void WriteNumber( char* data, const sbNumber& number ) const
  {
    *( double* )data = ( double )number.AsFloat();
  }
  virtual int GetElementSize() const { return sizeof( double ); }
  virtual int GetElementAlign() const { return ( int )__alignof( double ); };
};

class sbFieldF32 : public sbField
{
public:
  virtual sbNumber ReadNumber( const char* data ) const
  {
    return sbNumber::Float( *( float* )data );
  }
  virtual void WriteNumber( char* data, const sbNumber& number ) const
  {
    *( float* )data = ( float )number.AsFloat();
  }
  virtual int GetElementSize() const { return sizeof( float ); }
  virtual int GetElementAlign() const { return ( int )__alignof( float ); };
};

class sbFieldI64 : public sbField
{
public:
  virtual sbNumber ReadNumber( const char* data ) const
  {
    return sbNumber::Int( *( int64_t* )data );
  }
  virtual void WriteNumber( char* data, const sbNumber& number ) const
  {
    *( int64_t* )data = ( int64_t )number.AsInt();
  }
  virtual int GetElementSize() const { return sizeof( int64_t ); }
  virtual int GetElementAlign() const { return ( int )__alignof( int64_t ); };
};

class sbFieldU64 : public sbField
{
public:
  virtual sbNumber ReadNumber( const char* data ) const
  {
    return sbNumber::UInt( *( uint64_t* )data );
  }
  virtual void WriteNumber( char* data, const sbNumber& number ) const
  {
    *( uint64_t* )data = ( uint64_t )number.AsInt();
  }
  virtual int GetElementSize() const { return sizeof( uint64_t ); }
  virtual int GetElementAlign() const { return ( int )__alignof( uint64_t ); };
};

class sbFieldI32 : public sbField
{
public:
  virtual sbNumber ReadNumber( const char* data ) const
  {
    return sbNumber::Int( *( int32_t* )data );
  }
  virtual void WriteNumber( char* data, const sbNumber& number ) const
  {
    *( int32_t* )data = ( int32_t )number.AsInt();
  }
  virtual int GetElementSize() const { return sizeof( int32_t ); }
  virtual int GetElementAlign() const { return ( int )__alignof( int32_t ); };
};

class sbFieldU32 : public sbField
{
public:
  virtual sbNumber ReadNumber( const char* data ) const
  {
    return sbNumber::UInt( *( uint32_t* )data );
  }
  virtual void WriteNumber( char* data, const sbNumber& number ) const
  {
    *( uint32_t* )data = ( uint32_t )number.AsInt();
  }
  virtual int GetElementSize() const { return sizeof( uint32_t ); }
  virtual int GetElementAlign() const { return ( int )__alignof( uint32_t ); };
};

class sbFieldI16 : public sbField
{
public:
  virtual sbNumber ReadNumber( const char* data ) const
  {
    return sbNumber::Int( *( int16_t* )data );
  }
  virtual void WriteNumber( char* data, const sbNumber& number ) const
  {
    *( int16_t* )data = ( int16_t )number.AsInt();
  }
  virtual int GetElementSize() const { return sizeof( int16_t ); }
  virtual int GetElementAlign() const { return ( int )__alignof( int16_t ); };
};

class sbFieldU16 : public sbField
{
public:
  virtual sbNumber ReadNumber( const char* data ) const
  {
    return sbNumber::UInt( *( uint16_t* )data );
  }
  virtual void WriteNumber( char* data, const sbNumber& number ) const
  {
    *( uint16_t* )data = ( uint16_t )number.AsInt();
  }
  virtual int GetElementSize() const { return sizeof( uint16_t ); }
  virtual int GetElementAlign() const { return ( int )__alignof( uint16_t ); };
};

class sbFieldI8 : public sbField
{
public:
  virtual sbNumber ReadNumber( const char* data ) const
  {
    return sbNumber::Int( *( int8_t* )data );
  }
  virtual void WriteNumber( char* data, const sbNumber& number ) const
  {
    *( int8_t* )data = ( int8_t )number.AsInt();
  }
  virtual int GetElementSize() const { return sizeof( int8_t ); }
  virtual int GetElementAlign() const { return ( int )__alignof( int8_t ); };
};

class sbFieldU8 : public sbField
{
public:
  virtual sbNumber ReadNumber( const char* data ) const
  {
    return sbNumber::UInt( *( uint8_t* )data );
  }
  virtual void WriteNumber( char* data, const sbNumber& number ) const
  {
    *( uint8_t* )data = ( uint8_t )number.AsInt();
  }
  virtual int GetElementSize() const { return sizeof( uint8_t ); }
  virtual int GetElementAlign() const { return ( int )__alignof( uint8_t ); };
};

#endif
