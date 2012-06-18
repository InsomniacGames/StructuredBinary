//
//  sbAggregateType.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbAggregateType_h
#define StructuredBinary_sbAggregateType_h

#include <stddef.h>

#include "sbScalarType.h"
#include "sbValue.h"
#include "sbStatus.h"
#include "sbAllocator.h"
#include "sbHash.h"

#include "sbDictionary.h"

class sbSchema;
class sbScalarType;
class sbAggregateType;


class sbAggregateType : public sbType
{
  enum State
  {
    kState_Defined,
    kState_Fixing,
    kState_Ready,
  };

public:

  sbAggregateType()
  : m_Dictionary( 16, NULL )
  , m_State( kState_Defined )
  {}
  
  virtual ~sbAggregateType();

  virtual sbStatus Convert( char* dst_data, const char* src_data, const sbType* src_type, sbAllocator* alloc ) const;
  virtual size_t GetSize() const { return m_Size; }
  virtual size_t GetAlignment() const { return m_Alignment; }

  virtual bool IsTerminal( const char* data, const sbValue& terminator_value, sbHash terminator_name ) const;
  virtual sbStatus FixUp( sbSchema* schema );

  virtual const sbMember* FindMember( sbHash member_name ) const;
  virtual sbValue ReadValue( const char* data ) const;

  void AddInstance( sbHash member_name, int count, sbHash type_name );
  void AddPointer( sbHash member_name, int count, sbHash type_name, sbHash count_name );
  void AddString( sbHash member_name, int count, sbHash type_name, sbHash terminator_name, const sbValue& terminator_value );

private:
  void AddMember( sbHash member_name, sbMember* member );
  
  sbDictionary< sbMember* >  m_Dictionary;

  State             m_State;
  size_t            m_Size;
  size_t            m_Alignment;
};

#endif
