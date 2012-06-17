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
class sbNode;


class sbNode : public sbField
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
  
  virtual ~sbNode();

  virtual sbStatus Convert( char* dst_data, const char* src_data, const sbField* src_node, sbAllocator* alloc ) const;
  virtual size_t GetSize() const { return m_Size; }
  virtual size_t GetAlignment() const { return m_Alignment; }

  virtual int GetPointerCount( sbHash count_name, const char* data ) const;
  virtual int GetStringCount( const sbField* string_node, const sbScalarValue& terminator_value, sbHash terminator_name, const char* child_data ) const;
  virtual bool IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const;
  virtual sbStatus FixUp( sbSchema* schema );

  virtual const Child* FindChild( sbHash name ) const;
  virtual sbScalarValue ReadValue( const char* data ) const;

private:
  void AddInstance( sbHash name, int count, sbHash link_name );
  void AddPointer( sbHash name, int count, sbHash link_name, sbHash count_name );
  void AddString( sbHash name, int count, sbHash link_name, sbHash terminator_name, const sbScalarValue& terminator_value );
  friend class sbSchema;

  static const int  kMaxChildren = 100;


  Child*            m_Children[ kMaxChildren ];
  int               m_ChildCount;
  State             m_State;
  size_t            m_Size;
  size_t            m_Alignment;
  
};

#endif
