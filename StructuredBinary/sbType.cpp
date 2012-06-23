//
//  sbType.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/23/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "sbType.h"

#include "sbAggregateType.h"
#include "sbByteReader.h"

sbType* sbType::Read( sbByteReader* reader )
{
  sbType* sb_type = NULL;
  if( !sb_type ) sb_type = sbAggregateType::Read( reader );
  return sb_type;

}

