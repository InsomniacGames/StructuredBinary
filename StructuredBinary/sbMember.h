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
class sbValue;
class sbSchema;
class sbByteReader;
class sbByteWriter;

enum sbMemberType
{
  sbMemberType_Instance,
  sbMemberType_InstanceArray,
  sbMemberType_Pointer,
  sbMemberType_PointerArray,
  sbMemberType_CountedPointer,
  sbMemberType_CountedPointerArray,
  sbMemberType_StringPointer,
  sbMemberType_StringPointerArray,
};

class sbMember
{
public:
  sbMember( const sbAggregateType* scope, int count, sbHash type_name );
  virtual ~sbMember();
  
  char* GetDataPtr( char* scope_data, int index ) const;
  const char* GetDataPtr( const char* scope_data, int index ) const;
  sbValue ReadValue( const char* scope_data ) const;
  size_t GetTotalSize() const;
  sbStatus FixUp( sbSchema* schema, const sbMember* previous_member );
  
  virtual size_t GetSize() const = 0;
  virtual size_t GetAlignment() const = 0;
  virtual int GetPointerCount( const char* scope_data, int index ) const = 0;
  virtual sbStatus PreFixUp( sbSchema* schema, sbHash type_name ) = 0;
  virtual void Convert( char* dst_scope_data, const char* src_scope_data, const sbMember* src_member, sbAllocator* alloc ) const = 0;
  virtual void Write( sbByteWriter* writer ) = 0;

  size_t GetOffset() const { return m_Offset; }
  int GetCount() const { return m_Count; }
  const sbType* GetType() const { return m_Type; }
  const sbAggregateType* GetScope() const { return m_Scope; }
  sbHash GetTypeName() const { return m_TypeName; }

  static sbMember* Read( const sbAggregateType* scope, sbByteReader* reader );

private:
  sbHash   m_TypeName;
  size_t   m_Offset;     // Offset from the start of the node
  int      m_Count;      // For arrays
  const sbType*  m_Type;
  const sbAggregateType*   m_Scope;
};


#endif
