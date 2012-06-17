//
//  sbAggregate.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbAggregate_h
#define StructuredBinary_sbAggregate_h

#include <stddef.h>

#include "sbScalar.h"
#include "sbValue.h"
#include "sbStatus.h"
#include "sbAllocator.h"
#include "sbHash.h"

class sbSchema;
class sbScalar;
class sbAggregate;


class sbAggregate : public sbElement
{
  enum State
  {
    kState_Defined,
    kState_Fixing,
    kState_Ready,
  };

public:

  sbAggregate()
  : m_FirstField( NULL )
  , m_LastField( NULL )
  , m_State( kState_Defined )
  {}
  
  virtual ~sbAggregate();

  virtual sbStatus Convert( char* dst_element_data, const char* src_element_data, const sbElement* src_node, sbAllocator* alloc ) const;
  virtual size_t GetSize() const { return m_Size; }
  virtual size_t GetAlignment() const { return m_Alignment; }

  virtual bool IsTerminal( const char* element_data, const sbValue& terminator_value, sbHash terminator_name ) const;
  virtual sbStatus FixUp( sbSchema* schema );

  virtual const sbField* FindField( sbHash field_name ) const;
  virtual sbValue ReadValue( const char* data ) const;

private:
  void AddInstance( sbHash name, int count, sbHash element_name );
  void AddPointer( sbHash name, int count, sbHash element_name, sbHash count_name );
  void AddString( sbHash name, int count, sbHash element_name, sbHash terminator_name, const sbValue& terminator_value );
  void AddField( sbField* child );
  friend class sbSchema;

  sbField*            m_FirstField;
  sbField*            m_LastField;

  State             m_State;
  size_t            m_Size;
  size_t            m_Alignment;
};

#endif
