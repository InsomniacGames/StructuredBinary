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

  struct Entry
  {
    Entry() {}
    Entry( sbHash name, sbElement* field )
    : m_Name( name )
    , m_Node( field )
    {}

    sbHash  m_Name;
    sbElement* m_Node;
  };

  sbAggregate*  m_CurrentAggregate;
  sbHash        m_CurrentName;

  static const int  kMaxEntries = 100;
  int               m_EntryCount;
  Entry             m_Entries[ kMaxEntries ];
  
  static const Entry s_ScalarEntries[];
};

#endif
