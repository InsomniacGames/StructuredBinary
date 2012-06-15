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
#include "sbFnv.h"

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
  void AddScalar( sbHash name, int count, sbScalarType scalar_type );
  void AddInstance( sbHash name, int count, sbHash link_name );
  void AddPointer( sbHash name, int count, sbHash link_name, sbHash count_name );
  void AddString( sbHash name, int count, sbHash link_name, sbHash terminator_name, const sbScalarValue& terminator_value );
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

    sbHash   m_Name;
    sbHash   m_LinkName;
    sbHash   m_CountName;
    sbHash   m_TerminatorName;
    Type     m_Type;
    size_t   m_Offset;     // Offset from the start of the node
    size_t   m_ElementSize;
    size_t   m_ElementAlignment;
    int      m_Count;      // For arrays
    const sbScalar* m_Scalar;
    sbNode*       m_Node;
    sbScalarValue m_TerminatorValue;
  };

  Child             m_Children[ kMaxChildren ];
  int               m_ChildCount;
  State             m_State;
  size_t            m_Size;
  size_t            m_Alignment;
  
  const Child* FindChild( sbHash name ) const;                       // Virtual
  int GetPointerCount( const Child* child, const char* data ) const;      // Virtual
  int GetStringCount( const Child* child, const char* data ) const;       // Virtual

  bool IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const;  // Virtual
};

#endif
