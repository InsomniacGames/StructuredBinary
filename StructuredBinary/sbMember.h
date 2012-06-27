//
//  sbMember.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbMember_h
#define StructuredBinary_sbMember_h

#include <stddef.h>

#include "sbHash.h"
#include "sbStatus.h"

class sbAggregateType;
class sbType;
class sbAllocator;
class sbScalarValue;
class sbSchema;
class sbByteReader;
class sbByteWriter;

class sbMember
{
public:
  sbMember( int count, sbHash inline_type_name, sbHash indirect_type_name = 0U );
  virtual ~sbMember();
  
  char* GetDataPtr( char* scope_data, int index ) const;
  const char* GetDataPtr( const char* scope_data, int index ) const;
  sbScalarValue ReadScalarValue( const char* scope_data ) const;
  size_t GetTotalSize() const;
  sbStatus FixUp( sbSchema* schema, const sbMember* previous_member );
  
  virtual size_t GetSize() const = 0;
  virtual size_t GetAlignment() const = 0;
  virtual int GetPointerCount( const char* scope_data, int index ) const = 0;
  virtual sbStatus PreFixUp( sbSchema* schema, sbHash type_name ) = 0;
  virtual void Convert( char* dst_scope_data, const char* src_scope_data, const sbMember* src_member, sbAllocator* alloc ) const = 0;
  virtual uint64_t GetChecksum( uint64_t basis ) const;

  virtual void Write( sbByteWriter* writer ) const = 0;
  static sbMember* ReadNew( sbByteReader* reader );

  void SetScope( const sbAggregateType* scope );
  size_t GetOffset() const { return m_Offset; }
  int GetCount() const { return m_Count; }
  const sbType* GetInlineType() const { return m_InlineType; }
  const sbType* GetIndirectType() const { return m_IndirectType; }
  const sbAggregateType* GetScope() const { return m_Scope; }
  sbHash GetInlineTypeName() const { return m_InlineTypeName; }
  sbHash GetIndirectTypeName() const { return m_IndirectTypeName; }

private:
  sbHash    m_InlineTypeName;
  sbHash    m_IndirectTypeName;
  size_t    m_Offset;     // Offset from the start of the node
  int       m_Count;      // For arrays
  const sbType*           m_InlineType;
  const sbType*           m_IndirectType;
  const sbAggregateType*  m_Scope;

protected:

  enum ByteCode
  {
    ByteCode_Instance,
    ByteCode_CountedPointer,
    ByteCode_StringPointer,
    ByteCode_Mask = 0x07,
  };

  enum ByteCodeFlag
  {
    ByteCodeFlag_Array      = 1 << 7,   // Note: this means that the member is repeated in the aggregate
    ByteCodeFlag_CountName  = 1 << 6,   // Note: this means that the pointer points to more than one object. There will be a scalar member in the same aggregate that says how many
    ByteCodeFlag_TermName   = 1 << 5,
    ByteCodeFlag_TermValue  = 1 << 4,
  };
};

#endif
