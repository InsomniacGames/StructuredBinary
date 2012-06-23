//
//  sbInstanceMember.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbInstanceMember_h
#define StructuredBinary_sbInstanceMember_h

#include "sbMember.h"
#include "sbHash.h"

class sbAggregateType;
class sbAllocator;
class sbByteWriter;
class sbByteReader;

class sbInstanceMember : public sbMember
{
public:
  sbInstanceMember( const sbAggregateType* scope, int count, sbHash type_name );
  virtual size_t GetSize() const;
  virtual size_t GetAlignment() const;
  virtual int GetPointerCount( const char* scope_data, int index ) const;
  virtual void Convert( char* dst_data, const char* src_data, const sbMember* src_member, sbAllocator* alloc ) const;
  virtual sbStatus PreFixUp( sbSchema* schema, sbHash type_name );

  virtual void Write( sbByteWriter* writer ) const;
  static sbMember* Read( sbByteReader* reader, const sbAggregateType* scope );
};


#endif
