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

class sbField;
class sbByteReader;
class sbByteWriter;

enum FieldType
{
  kField_Unknown = 0,
  kField_I8,
  kField_U8,
  kField_I16,
  kField_U16,
  kField_I32,
  kField_U32,
  kField_I64,
  kField_U64,
  kField_F32,
  kField_F64,
  kField_Agg,
  kField_Count
};

typedef const sbField* (BuildField)( sbByteReader* reader );

struct FieldInfo
{
  FieldType   field_type;
  const char* description;
  BuildField* build_field;
};

const FieldInfo& GetInfo( FieldType field_type );

struct WriteCursor
{
  WriteCursor( char* data, uint32_t name, const sbField* field )
  : m_Name( name )
  , m_Data( data )
  , m_Field( field )
  {}
  uint32_t      m_Name; // Remove me
  char*         m_Data;
  const sbField*  m_Field;
};

struct ReadCursor
{
  ReadCursor( const char* data, uint32_t name, const sbField* field )
  : m_Name( name )
  , m_Data( data )
  , m_Field( field )
  {}
  uint32_t      m_Name; // Remove me
  const char*   m_Data;
  const sbField*  m_Field;
};

class sbField
{
public:
  virtual ~sbField() {}
  virtual sbNumber ReadNumber( const char* data ) const { return sbNumber::Null(); }
  virtual void WriteNumber( char* data, const sbNumber& number ) const {}

  virtual void Convert( char* dst_data, const ReadCursor& rc ) const
  {
    sbNumber n = rc.m_Field->ReadNumber( rc.m_Data );
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
  virtual int GetElementAlign() const = 0;
  virtual FieldType GetType() const = 0;

private:
};

class FieldFloat64 : public sbField
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
  virtual FieldType GetType() const { return kField_F64; }
};

class FieldFloat32 : public sbField
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
  virtual FieldType GetType() const { return kField_F32; }
};

class FieldInt64 : public sbField
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
  virtual FieldType GetType() const { return kField_I64; }
};

class FieldUInt64 : public sbField
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
  virtual FieldType GetType() const { return kField_U64; }
};

class FieldInt32 : public sbField
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
  virtual FieldType GetType() const { return kField_I32; }
};

class FieldUInt32 : public sbField
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
  virtual FieldType GetType() const { return kField_U32; }
};

class FieldInt16 : public sbField
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
  virtual FieldType GetType() const { return kField_I16; }
};

class FieldUInt16 : public sbField
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
  virtual FieldType GetType() const { return kField_U16; }
};

class FieldInt8 : public sbField
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
  virtual FieldType GetType() const { return kField_I8; }
};

class FieldUInt8 : public sbField
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
  virtual FieldType GetType() const { return kField_U8; }
};

#endif
