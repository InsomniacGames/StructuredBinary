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

class sbSchema;
class sbScalar;
class sbScalarValue;
class sbPath;

enum sbScalarType
{
  kScalar_Null,
  kScalar_U8,
  kScalar_I8,
  kScalar_U16,
  kScalar_I16,
  kScalar_U32,
  kScalar_I32,
  kScalar_U64,
  kScalar_I64,
  kScalar_F32,
  kScalar_F64,
};

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

  void Convert( char* dst_data, const char* src_data, const sbNode* src_node ) const;

  void AddScalar( const char* name, int count, sbScalarType scalar_type );
  void AddInstance( const char* name, int count, const char* link_name );
  void AddPointer( const char* name, int count, const char* link_name, const char* count_name );

  void PrintNode( const char* data, const sbPath* parent ) const;

  void FixUp( sbSchema* schema );

  size_t GetSize() const { return m_Size; }
  size_t GetAlignment() const { return m_Alignment; }

private:
  static const int  kMaxChildren = 100;

  struct Child
  {
    enum Type
    {
      kType_Scalar,
      kType_Pointer,
      kType_Instance,
    };

    const char*   m_Name;
    const char*   m_LinkName;
    const char*   m_CountName;
    Type          m_Type;
    size_t        m_Offset;     // Offset from the start of the node
    int           m_Count;      // For arrays
    const sbScalar* m_Scalar;
    sbNode*       m_Node;
  };

  Child             m_Children[ kMaxChildren ];
  int               m_ChildCount;
  State             m_State;
  size_t            m_Size;
  size_t            m_Alignment;
  
  const Child* FindChild( const char* name ) const;
};

#endif
