//
//  sbPointerType.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/24/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbPointerType_h
#define StructuredBinary_sbPointerType_h

#include "sbType.h"
#include "sbHash.h"

class sbByteWriter;
class sbByteReader;
class sbPointerValue;

class sbPointerType : public sbType
{
public:
  sbPointerType( const sbType* indirect_type );

  virtual sbStatus ConvertOne( char* dst_data, const char* src_data, const sbType* src_type, class sbAllocator* alloc, int array_count ) const;
  virtual size_t GetSize() const;
  virtual size_t GetAlignment() const;

  virtual bool IsTerminal( const char* data, const sbScalarValue& terminator_value, sbHash terminator_name ) const;

  virtual bool IsBuiltIn() const { return true; }
  virtual void Write( sbByteWriter* writer ) const {} // All pointer types are built-in
  virtual uint64_t GetChecksum( uint64_t basis ) const { return basis; }

  virtual sbStatus FixUp( class sbSchema* schema );

  virtual sbPointerValue ReadPointerValue( const char* data ) const;
  virtual void WritePointerValue( char* data, const sbPointerValue& value ) const;

  virtual const sbType* GetIndirectType() const { return m_IndirectType; }

  virtual const sbPointerType* AsPointerType() const { return this; }

private:
  const sbType* m_IndirectType;
};

#endif
