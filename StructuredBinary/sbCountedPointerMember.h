//
//  sbCountedPointerMember.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbCountedPointerMember_h
#define StructuredBinary_sbCountedPointerMember_h

#include "sbPointerMember.h"

class sbByteWriter;
class sbByteReader;

class sbCountedPointerMember : public sbPointerMember
{
  sbHash   m_CountName;
  
public:
  sbCountedPointerMember( int count, sbHash type_name, sbHash count_name );
  virtual int GetPointerCount( const char* scope_data, int index ) const;
  virtual uint64_t GetChecksum( uint64_t basis ) const;

  virtual void Write( sbByteWriter* writer ) const;
  static sbCountedPointerMember* ReadNew( sbByteReader* reader );
};

#endif
