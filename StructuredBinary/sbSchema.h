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

class sbScalarValue;
class sbAllocator;
class sbAggregateType;
class sbByteReader;
class sbByteWriter;

class sbSchema
{
public:
  
  sbSchema();
  ~sbSchema();

  const sbType* FindType( sbHash name ) const;
  sbType* FindType( sbHash name );

  void Begin();
  void End();

  void BeginAggregate( sbHash type_name );
  void EndAggregate();

  char* Convert( const char* src_data, const sbSchema* src_schema, sbHash name, sbAllocator* alloc ) const;

  void AddInstance( sbHash member_name, int count, sbHash type_name );
  void AddPointer( sbHash member_name, int count, sbHash type_name );
  void AddCountedPointer( sbHash member_name, int count, sbHash type_name, sbHash count_name );
  void AddStringPointer( sbHash member_name, int count, sbHash type_name, sbHash terminator_name, const sbScalarValue& terminator_value );

  sbStatus FixUp( sbHash type_name );

  void Write( sbByteWriter* writer ) const;
  static sbSchema* ReadNew( sbByteReader* reader );
  uint64_t GetChecksum( uint64_t basis = 0 ) const;

private:

  enum State
  {
    kState_New,
    kState_Building,
    kState_Ready,
  };

  State     m_State;

  sbStatus FixUp();
  void AddType( sbHash type_name, sbType* sb_type );

  sbAggregateType*  m_CurrentAggregate;
  sbHash        m_CurrentName;

  sbDictionary< sbType* > m_Dictionary;

  void ConvertAll( sbAllocator* alloc ) const;
};

#endif
