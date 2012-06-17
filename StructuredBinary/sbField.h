//
//  sbField.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbField_h
#define StructuredBinary_sbField_h

#include <stddef.h>

#include "sbHash.h"
#include "sbStatus.h"

class sbAggregate;
class sbElement;
class sbAllocator;
class sbValue;
class sbSchema;

class sbField
{
public:
  sbField( const sbAggregate* aggregate, sbHash field_name, int count, sbHash element_name );
  virtual ~sbField();
  
  char* GetDataPtr( char* aggregate_data, int index ) const;
  const char* GetDataPtr( const char* aggregate_data, int index ) const;
  sbValue ReadValue( const char* aggregate_data ) const;
  size_t GetTotalSize() const;
  
  virtual size_t GetSize() const = 0;
  virtual size_t GetAlignment() const = 0;
  virtual int GetPointerCount( const char* aggregate_data, int index ) const = 0;
  virtual sbStatus FixUp( sbSchema* schema, const sbField* previous_field ) = 0;
  virtual void Convert( char* dst_aggregate_data, const char* src_aggregate_data, const sbField* src_field, sbAllocator* alloc ) const = 0;

  sbHash GetName() const { return m_FieldName; }
  sbField* GetNext() const { return m_Next; }

//private:
  sbHash   m_FieldName;
  sbHash   m_ElementName;
  size_t   m_Offset;     // Offset from the start of the node
  int      m_Count;      // For arrays
  const sbElement*  m_Element;
  sbField*   m_Next;
  const sbAggregate*   m_Aggregate;
};


#endif
