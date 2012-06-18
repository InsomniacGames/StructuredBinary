//
//  sbSchema.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbSchema_h
#define StructuredBinary_sbSchema_h

#include <cstring>

#include "sbStatus.h"
#include "sbScalarType.h"
#include "sbHash.h"
#include "sbDictionary.h"

class sbValue;
class sbAllocator;
class sbAggregateType;

class sbSchema
{
public:
  
  sbSchema();
  ~sbSchema();

  const sbType* FindType( sbHash name ) const;
  sbType* FindType( sbHash name );

  void Begin();
  void End();

  void BeginType( sbHash type_name );
  void EndType();

  sbStatus Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, sbHash name, sbAllocator* alloc ) const;

  void AddInstance( sbHash member_name, int count, sbHash type_name );
  void AddPointer( sbHash member_name, int count, sbHash type_name, sbHash count_name );
  void AddString( sbHash member_name, int count, sbHash type_name, sbHash terminator_name, const sbValue& terminator_value );

  sbStatus FixUp( sbHash type_name );

private:
  
  enum State
  {
    kState_New,
    kState_Building,
    kState_Ready,
  };
  
  State m_State;

  sbStatus FixUp();

  sbAggregateType*  m_CurrentAggregate;
  sbHash        m_CurrentName;

  sbDictionary< sbType* > m_Dictionary;
};

#endif
