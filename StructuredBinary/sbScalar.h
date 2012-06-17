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
#include <stdio.h>

// Project
#include "sbScalarValue.h"
#include "sbFnv.h"
#include "sbStatus.h"
#include "sbAllocator.h"




class sbField
{
public:
  virtual ~sbField() {}
  
  virtual sbStatus Convert( char* dst_data, const char* src_data, const sbField* src_node, class sbAllocator* alloc ) const = 0;
  virtual size_t GetSize() const = 0;
  virtual size_t GetAlignment() const = 0;
  
  virtual int GetPointerCount( sbHash count_name, const char* data ) const = 0;
  virtual int GetStringCount( const sbField* string_node, const sbScalarValue& terminator_value, sbHash terminator_name, const char* child_data ) const = 0;
  virtual bool IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const = 0;
  
  virtual sbStatus FixUp( class sbSchema* schema ) = 0;
  virtual const class Child* FindChild( sbHash name ) const = 0;
  
  virtual sbScalarValue ReadValue( const char* data ) const = 0;
};



class Child
{
public:
  sbHash   m_Name;
  sbHash   m_LinkName;
  sbHash   m_CountName;
  sbHash   m_TerminatorName;
  size_t   m_Offset;     // Offset from the start of the node
  size_t   m_ElementSize;
  size_t   m_ElementAlignment;
  int      m_Count;      // For arrays
  class sbField*  m_Node;
  sbScalarValue m_TerminatorValue;

  virtual size_t GetSize() const = 0;
  virtual size_t GetAlignment() const = 0;
  virtual int GetStringCount( const sbField* parent_node, const char* data ) const = 0;
  virtual int GetPointerCount( const sbField* parent_node, const char* parent_data ) const = 0;

  virtual void Convert( char* dst_data, const char* src_data, const sbField* src_node, sbAllocator* alloc ) const = 0;
};

class ChildInstance : public Child
{
public:
  virtual size_t GetSize() const
  {
    return m_Node->GetSize();
  }
  
  virtual size_t GetAlignment() const
  {
    return m_Node->GetAlignment();
  }

  virtual int GetStringCount( const sbField* parent_node, const char* data ) const
  {
    return 0;
  }

  virtual int GetPointerCount( const sbField* parent_node, const char* parent_data ) const
  {
    return 0;
  }

  virtual void Convert( char* dst_data, const char* src_data, const sbField* src_node, sbAllocator* alloc ) const
  {
    const Child* src_child = src_node->FindChild( m_Name );
    for( int index = 0; index < m_Count; ++index )
    {
      char* dst_child_data = dst_data ? dst_data + m_Offset + index * m_ElementSize : NULL;
      const char* src_child_data = src_data + src_child->m_Offset + index * src_child->m_ElementSize;
      m_Node->Convert( dst_child_data, src_child_data, src_child->m_Node, alloc );
    }
  }
};

class ChildPointer : public Child
{
public:
  virtual size_t GetSize() const
  {
    return sizeof( void* );
  }
  
  virtual size_t GetAlignment() const
  {
    return __alignof( void* );
  }

  virtual int GetStringCount( const sbField* parent_node, const char* data ) const
  {
    return 0;
  }

  virtual int GetPointerCount( const sbField* parent_node, const char* parent_data ) const
  {
    return parent_node->GetPointerCount( m_CountName, parent_data );
  }

  virtual void Convert( char* dst_data, const char* src_data, const sbField* src_node, sbAllocator* alloc ) const
  {
    const Child* src_child = src_node->FindChild( m_Name );
    for( int index = 0; index < m_Count; ++index )
    {
      char* dst_child_data = dst_data ? dst_data + m_Offset + index * m_ElementSize : NULL;
      const char* src_child_data = src_data + src_child->m_Offset + index * src_child->m_ElementSize;
      int element_count = src_child->GetPointerCount( src_node, src_data );
      
      size_t dst_size = m_Node->GetSize();
      size_t src_size = src_child->m_Node->GetSize();
      
      const char* src_p = *( const char** )( src_child_data );
      sbAllocator::Result r = alloc->Alloc( dst_size, element_count, m_Node->GetAlignment(), src_p );
      char* dst_p = r.m_Data;
      if( dst_child_data )
      {
        *( char** )( dst_child_data ) = dst_p;
      }
      
      if( !r.m_Done )
      {
        for( int j = 0; j < element_count; ++j )
        {
          m_Node->Convert( dst_p, src_p, src_child->m_Node, alloc );
          src_p += src_size;
          dst_p += dst_p ? dst_size : 0;
        }
      }
    }
  }
};

class ChildString : public Child
{
public:
  virtual size_t GetSize() const
  {
    return sizeof( void* );
  }
  
  virtual size_t GetAlignment() const
  {
    return __alignof( void* );
  }

  virtual int GetStringCount( const sbField* parent_node, const char* data ) const
  {
    return parent_node->GetStringCount( m_Node, m_TerminatorValue, m_TerminatorName, data );
  }

  virtual int GetPointerCount( const sbField* parent_node, const char* parent_data ) const
  {
    return 0;
  }

  virtual void Convert( char* dst_data, const char* src_data, const sbField* src_node, sbAllocator* alloc ) const
  {
    const Child* src_child = src_node->FindChild( m_Name );
    for( int index = 0; index < m_Count; ++index )
    {
      char* dst_child_data = dst_data ? dst_data + m_Offset + index * m_ElementSize : NULL;
      const char* src_child_data = src_data + src_child->m_Offset + index * src_child->m_ElementSize;
      int element_count = src_child->GetStringCount( src_node, src_child_data );
      size_t dst_size = m_Node->GetSize();
      size_t src_size = src_child->m_Node->GetSize();
      
      const char* src_p = *( const char** )( src_child_data );
      sbAllocator::Result r = alloc->Alloc( dst_size, element_count, m_Node->GetAlignment(), src_p );
      char* dst_p = r.m_Data;
      if( dst_child_data )
      {
        *( char** )( dst_child_data ) = dst_p;
      }
      
      if( !r.m_Done )
      {
        for( int j = 0; j < element_count; ++j )
        {
          m_Node->Convert( dst_p, src_p, src_child->m_Node, alloc );
          src_p += src_size;
          dst_p += dst_p ? dst_size : 0;
        }
      }
    }
  }
};















class sbScalar : public sbField
{
public:
  virtual ~sbScalar() {}
  virtual const sbScalar* AsScalar() const { return this; }

  virtual sbScalarValue ReadValue( const char* data ) const { return sbScalarValue::Null(); }
  virtual void WriteValue( char* data, const sbScalarValue& number ) const {}

  virtual void Convert( char* write_data, const char* read_data, const sbScalar* read_scalar ) const
  {
    sbScalarValue value = read_scalar->ReadValue( read_data );
    WriteValue( write_data, value );
  }

  virtual size_t GetSize() const = 0;
  virtual size_t GetAlignment() const = 0;
  
  //------------
  virtual sbStatus Convert( char* dst_child_data, const char* src_child_data, const sbField* src_node, class sbAllocator* alloc ) const
  {
    sbScalarValue src_value = src_node->ReadValue( src_child_data );
    if( dst_child_data )
    {
      WriteValue( dst_child_data, src_value );
    }
    return sbStatus_Ok;
  }

  // -----------------
  virtual int GetPointerCount( sbHash count_name, const char* data ) const { return 0; }
  virtual int GetStringCount( const sbField* string_node, const sbScalarValue& terminator_value, sbHash terminator_name, const char* child_data ) const { return 0; }
  virtual bool IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const
  {
    sbScalarValue value = ReadValue( data );
    return value == terminator_value;
  }

  virtual sbStatus FixUp( class sbSchema* schema ) { return sbStatus_Ok; }
  virtual const Child* FindChild( sbHash name ) const { return NULL; }
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
  virtual size_t GetSize() const { return sizeof( double ); }
  virtual size_t GetAlignment() const { return __alignof( double ); };
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
  virtual size_t GetSize() const { return sizeof( float ); }
  virtual size_t GetAlignment() const { return __alignof( float ); };
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
  virtual size_t GetSize() const { return sizeof( int64_t ); }
  virtual size_t GetAlignment() const { return __alignof( int64_t ); };
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
  virtual size_t GetSize() const { return sizeof( uint64_t ); }
  virtual size_t GetAlignment() const { return __alignof( uint64_t ); };
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
  virtual size_t GetSize() const { return sizeof( int32_t ); }
  virtual size_t GetAlignment() const { return __alignof( int32_t ); };
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
  virtual size_t GetSize() const { return sizeof( uint32_t ); }
  virtual size_t GetAlignment() const { return __alignof( uint32_t ); };
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
  virtual size_t GetSize() const { return sizeof( int16_t ); }
  virtual size_t GetAlignment() const { return __alignof( int16_t ); };
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
  virtual size_t GetSize() const { return sizeof( uint16_t ); }
  virtual size_t GetAlignment() const { return __alignof( uint16_t ); };
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
  virtual size_t GetSize() const { return sizeof( int8_t ); }
  virtual size_t GetAlignment() const { return __alignof( int8_t ); };
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
  virtual size_t GetSize() const { return sizeof( uint8_t ); }
  virtual size_t GetAlignment() const { return __alignof( uint8_t ); };
};


#endif
