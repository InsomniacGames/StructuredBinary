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
#include "sbScalar.h"
#include "sbHash.h"
#include "sbDictionary.h"

class sbValue;
class sbAllocator;
class sbAggregate;

class sbSchema
{
public:
  
  sbSchema();
  ~sbSchema();

  const sbElement* FindElement( sbHash name ) const;
  sbElement* FindElement( sbHash name );

  void Begin();
  void End();

  void BeginElement( sbHash element_name );
  void EndElement();

  sbStatus Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, sbHash name, sbAllocator* alloc ) const;

  void AddInstance( sbHash field_name, int count, sbHash element_name );
  void AddPointer( sbHash field_name, int count, sbHash element_name, sbHash count_name );
  void AddString( sbHash field_name, int count, sbHash element_name, sbHash terminator_name, const sbValue& terminator_value );

  sbStatus FixUp( sbHash element_name );

private:
  
  enum State
  {
    kState_New,
    kState_Building,
    kState_Ready,
  };
  
  State m_State;

  sbStatus FixUp();

  sbAggregate*  m_CurrentAggregate;
  sbHash        m_CurrentName;

  sbDictionary< sbElement* > m_Dictionary;
};

#endif
