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
#include "sbFnv.h"

class sbNode;
class sbScalarValue;
class sbAllocator;

class sbSchema
{
public:
  
  sbSchema()
  : m_State( kState_New )
  , m_CurrentNode( NULL)
  , m_EntryCount( 0 )
  {}

  ~sbSchema();

  const sbNode* FindNode( sbHash name ) const;
  sbNode* FindNode( sbHash name );

  void Begin();
  void End();

  void BeginNode( sbHash name );
  void EndNode();

  sbStatus Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, sbHash name, sbAllocator* alloc ) const;

  void AddScalar( sbHash name, int count, sbScalarType scalar_type );
  void AddInstance( sbHash name, int count, sbHash link_name );
  void AddPointer( sbHash name, int count, sbHash link_name, sbHash count_name );
  void AddString( sbHash name, int count, sbHash link_name, sbHash terminator_name, const sbScalarValue& terminator_value );

  void AddPointer( sbHash name, int count, sbScalarType t, sbHash count_name );
  void AddString( sbHash name, int count, sbScalarType t, const sbScalarValue& terminator_value );

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
    sbHash  m_Name;
    sbNode* m_Node;
  };

  sbNode* m_CurrentNode;
  sbHash m_CurrentName;

  static const int  kMaxEntries = 100;
  int               m_EntryCount;
  Entry             m_Entries[ kMaxEntries ];
};

#endif
