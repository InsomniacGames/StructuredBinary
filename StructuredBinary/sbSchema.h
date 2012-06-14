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

class sbNode;
class sbScalarValue;
class sbAllocator;

class sbSchema
{
public:
  
  sbSchema()
  : m_State( kState_New )
  , m_CurrentNode( NULL)
  , m_CurrentName( NULL )
  , m_EntryCount( 0 )
  {}

  ~sbSchema();

  const sbNode* FindNode( const char* name ) const;
  sbNode* FindNode( const char* name );

  void Begin();
  void End();

  void BeginNode( const char* name );
  void EndNode();

  sbStatus Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, const char* name, sbAllocator* alloc ) const;

  void AddScalar( const char* name, int count, sbScalarType scalar_type );
  void AddInstance( const char* name, int count, const char* link_name );
  void AddPointer( const char* name, int count, const char* link_name, const char* count_name );
  void AddString( const char* name, int count, const char* link_name, const char* terminator_name, const sbScalarValue& terminator_value );

  void AddPointer( const char* name, int count, sbScalarType t, const char* count_name );
  void AddString( const char* name, int count, sbScalarType t, const sbScalarValue& terminator_value );
  
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
    const char* m_Name;
    sbNode*     m_Node;
  };

  sbNode* m_CurrentNode;
  const char* m_CurrentName;

  static const int  kMaxEntries = 100;
  int               m_EntryCount;
  Entry             m_Entries[ kMaxEntries ];
};

#endif
