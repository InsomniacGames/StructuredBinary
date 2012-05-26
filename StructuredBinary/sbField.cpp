//
//  sbField.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include <assert.h>

#include "sbField.h"
#include "sbStruct.h"

class FieldF64 : public sbField
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
  virtual sbFieldType GetType() const { return kField_F64; }

  virtual void WriteSchema( sbByteWriter* writer ) const
  {
    writer->Write8( kField_F64 );
  }

  static const sbField* ReadSchema( sbByteReader* reader )
  {
    return new FieldF64;
  }
};

class FieldF32 : public sbField
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
  virtual sbFieldType GetType() const { return kField_F32; }

  virtual void WriteSchema( sbByteWriter* writer ) const
  {
    writer->Write8( kField_F32 );
  }

  static const sbField* ReadSchema( sbByteReader* reader )
  {
    return new FieldF32;
  }
};

class FieldI64 : public sbField
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
  virtual sbFieldType GetType() const { return kField_I64; }

  virtual void WriteSchema( sbByteWriter* writer ) const
  {
    writer->Write8( kField_I64 );
  }

  static const sbField* ReadSchema( sbByteReader* reader )
  {
    return new FieldI64;
  }
};

class FieldU64 : public sbField
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
  virtual sbFieldType GetType() const { return kField_U64; }

  virtual void WriteSchema( sbByteWriter* writer ) const
  {
    writer->Write8( kField_U64 );
  }

  static const sbField* ReadSchema( sbByteReader* reader )
  {
    return new FieldU64;
  }
};

class FieldI32 : public sbField
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
  virtual sbFieldType GetType() const { return kField_I32; }

  virtual void WriteSchema( sbByteWriter* writer ) const
  {
    writer->Write8( kField_I32 );
  }

  static const sbField* ReadSchema( sbByteReader* reader )
  {
    return new FieldI32;
  }
};

class FieldU32 : public sbField
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
  virtual sbFieldType GetType() const { return kField_U32; }

  virtual void WriteSchema( sbByteWriter* writer ) const
  {
    writer->Write8( kField_U32 );
  }

  static const sbField* ReadSchema( sbByteReader* reader )
  {
    return new FieldU32;
  }
};

class FieldI16 : public sbField
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
  virtual sbFieldType GetType() const { return kField_I16; }

  virtual void WriteSchema( sbByteWriter* writer ) const
  {
    writer->Write8( kField_I16 );
  }

  static const sbField* ReadSchema( sbByteReader* reader )
  {
    return new FieldI16;
  }
};

class FieldU16 : public sbField
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
  virtual sbFieldType GetType() const { return kField_U16; }

  virtual void WriteSchema( sbByteWriter* writer ) const
  {
    writer->Write8( kField_U16 );
  }

  static const sbField* ReadSchema( sbByteReader* reader )
  {
    return new FieldU16;
  }
};

class FieldI8 : public sbField
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
  virtual sbFieldType GetType() const { return kField_I8; }

  virtual void WriteSchema( sbByteWriter* writer ) const
  {
    writer->Write8( kField_I8 );
  }

  static const sbField* ReadSchema( sbByteReader* reader )
  {
    return new FieldI8;
  }
};

class FieldU8 : public sbField
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
  virtual sbFieldType GetType() const { return kField_U8; }

  virtual void WriteSchema( sbByteWriter* writer ) const
  {
    writer->Write8( kField_U8 );
  }

  static const sbField* ReadSchema( sbByteReader* reader )
  {
    return new FieldU8;
  }
};

static const sbField* BuildUnknown( sbByteReader* reader )
{
  assert( false );
  return NULL;
}

const sbFieldTypeInfo g_FieldInfo[ kField_Count ] =
{
  { kField_Unknown, "unknown",  &BuildUnknown         },
  { kField_I8,      "int8_t",   &FieldI8::ReadSchema  },
  { kField_U8,      "uint8_t",  &FieldU8::ReadSchema  },
  { kField_I16,     "int16_t",  &FieldI16::ReadSchema },
  { kField_U16,     "uint16_t", &FieldU16::ReadSchema },
  { kField_I32,     "int32_t",  &FieldI32::ReadSchema },
  { kField_U32,     "uint32_t", &FieldU32::ReadSchema },
  { kField_I64,     "int64_t",  &FieldI64::ReadSchema },
  { kField_U64,     "uint64_t", &FieldU64::ReadSchema },
  { kField_F32,     "float",    &FieldF32::ReadSchema },
  { kField_F64,     "double",   &FieldF64::ReadSchema },
  { kField_Agg,     "agg",      &sbStruct::ReadSchema },
};

const sbFieldTypeInfo& GetInfo( sbFieldType field_type )
{
  assert( field_type >= 0 && field_type < kField_Count );
  assert( g_FieldInfo[ field_type ].field_type == field_type );
  return g_FieldInfo[ field_type ];
}

