//
//  sbType.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/23/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbType.h"

#include <assert.h>

#include "sbAggregateType.h"
#include "sbByteReader.h"
#include "sbPointerValue.h"
#include "sbScalarValue.h"

sbType* sbType::ReadNew( sbByteReader* reader )
{
  sbType* sb_type = NULL;
  if( !sb_type ) sb_type = reader->ReadNew< sbAggregateType >();
  return sb_type;
}

sbScalarValue sbType::ReadScalarValue( const char* data ) const
{
  // TO DO: probably should report error and bail out
  assert( false );
}

sbPointerValue sbType::ReadPointerValue( const char* data ) const
{
  // TO DO: probably should report error and bail out
  assert( false );
}

const class sbMember* sbType::FindMember( sbHash name ) const
{
  // TO DO: probably should report error and bail out
  assert( false );
}

