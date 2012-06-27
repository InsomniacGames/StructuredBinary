//
//  sbType.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbType_h
#define StructuredBinary_sbType_h

#include <stdint.h>
#include <stddef.h>

#include "sbStatus.h"
#include "sbHash.h"

class sbByteWriter;
class sbByteReader;
class sbScalarValue;
class sbPointerValue;

struct sbDestArray
{
  sbDestArray( const class sbType* t, char* data, int count )
  : m_Type( t )
  , m_Data( data )
  , m_Count( count )
  {}

  const class sbType* m_Type;
  char*               m_Data;
  int                 m_Count;
};

struct sbSourceArray
{
  sbSourceArray( const class sbType* t, const char* data, int count )
  : m_Type( t )
  , m_Data( data )
  , m_Count( count )
  {}

  const class sbType* m_Type;
  const char*         m_Data;
  int                 m_Count;
};


class sbType
{
public:
  static sbStatus ConvertMany( const sbDestArray& dst_array, const sbSourceArray& src_array, class sbAllocator* alloc );

  virtual ~sbType() {}
  
  virtual sbStatus ConvertOne( char* dst_data, const char* src_data, const sbType* src_type, class sbAllocator* alloc ) const = 0;
  virtual size_t GetSize() const = 0;
  virtual size_t GetAlignment() const = 0;
  
  virtual bool IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const = 0;
  
  virtual sbStatus FixUp( class sbSchema* schema ) = 0;

  // Implemented in certain subclasses only. Default implementation asserts (needs more flexible error reporting!)
  virtual const class sbMember* FindMember( sbHash name ) const;
  virtual sbScalarValue ReadScalarValue( const char* data ) const;
  virtual sbPointerValue ReadPointerValue( const char* data ) const;

  virtual bool IsBuiltIn() const = 0;
  virtual void Write( sbByteWriter* writer ) const = 0;
  static sbType* ReadNew( sbByteReader* reader );
  virtual uint64_t GetChecksum( uint64_t basis ) const = 0;

protected:

  enum ByteCode
  {
    ByteCode_Aggregate,
    ByteCode_Scalar,
  };
};

#endif
