//
//  sbStringPointerMember.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbStringPointerMember_h
#define StructuredBinary_sbStringPointerMember_h

#include "sbPointerMember.h"

#include "sbScalarValue.h"
#include "sbHash.h"

class sbAggregateType;
class sbByteWriter;
class sbByteReader;

class sbStringPointerMember : public sbPointerMember
{
  sbScalarValue m_TerminatorValue;
  sbHash        m_TerminatorName;
  
public:
  sbStringPointerMember( int count, sbHash type_name, sbHash terminator_name, const sbScalarValue& terminator_value );
  virtual int GetPointerCount( const char* scope_data, int index ) const;
  virtual uint64_t GetChecksum( uint64_t basis ) const;

  virtual void Write( sbByteWriter* writer ) const;
  static sbStringPointerMember* ReadNew( sbByteReader* reader );
};

#endif
