//
//  sbPointerMember.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbPointerMember_h
#define StructuredBinary_sbPointerMember_h

#include <stddef.h>

#include "sbMember.h"
#include "sbHash.h"
#include "sbPointerType.h"

class sbAggregateType;
class sbAllocator;
class sbByteWriter;
class sbByteReader;

class sbPointerMember : public sbMember
{  
public:
  sbPointerMember( int count, sbHash type_name );
  virtual ~sbPointerMember();
  virtual size_t GetSize() const;
  virtual size_t GetAlignment() const;
  virtual void Convert( char* dst_scope_data, const char* src_scope_data, const sbMember* src_member, sbAllocator* alloc ) const;
  virtual const sbType* PreFixUp( sbSchema* schema, sbHash type_name );

private:
  const sbType* m_Type;
};

#endif
