//
//  sbNode.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbNode_h
#define StructuredBinary_sbNode_h

#include <stddef.h>

#include "sbScalar.h"
#include "sbScalarValue.h"
#include "sbStatus.h"
#include "sbAllocator.h"

class sbSchema;
class sbScalar;

class sbNode
{
  enum State
  {
    kState_Defined,
    kState_Fixing,
    kState_Ready,
  };

public:

  sbNode()
  : m_ChildCount( 0 )
  , m_State( kState_Defined )
  {}

  sbStatus Convert( char* dst_data, const char* src_data, const sbNode* src_node, sbAllocator* alloc ) const;   // Virtual


  size_t GetSize() const { return m_Size; }                     // Virtual
  size_t GetAlignment() const { return m_Alignment; }           // Virtual

private:
  void AddScalar( const char* name, int count, sbScalarType scalar_type );
  void AddInstance( const char* name, int count, const char* link_name );
  void AddPointer( const char* name, int count, const char* link_name, const char* count_name );
  void AddString( const char* name, int count, const char* link_name, const sbScalarValue& terminator, const char* terminator_name );
  sbStatus FixUp( sbSchema* schema );                           // Virtual
  friend class sbSchema;  // TEMP

  static const int  kMaxChildren = 100;

  struct Child
  {
    enum Type
    {
      kType_Scalar,
      kType_Pointer,
      kType_String,
      kType_Instance,
    };

    const char*   m_Name;
    const char*   m_LinkName;
    const char*   m_CountName;
    const char*   m_TerminatorName;
    Type          m_Type;
    size_t        m_Offset;     // Offset from the start of the node
    size_t        m_ElementSize;
    size_t        m_ElementAlignment;
    int           m_Count;      // For arrays
    const sbScalar* m_Scalar;
    sbNode*       m_Node;
    sbScalarValue m_Terminator;
  };

  Child             m_Children[ kMaxChildren ];
  int               m_ChildCount;
  State             m_State;
  size_t            m_Size;
  size_t            m_Alignment;
  
  const Child* FindChild( const char* name ) const;                       // Virtual
  int GetPointerCount( const Child* child, const char* data ) const;      // Virtual
  int GetStringCount( const Child* child, const char* data ) const;       // Virtual

  bool IsTerminal( const char* data, const sbScalarValue& terminator_value, const char* terminator_name ) const;  // Virtual
};

#endif
