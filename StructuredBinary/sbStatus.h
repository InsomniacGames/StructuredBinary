//
//  sbStatus.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/9/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbStatus_h
#define StructuredBinary_sbStatus_h

enum sbStatus
{
  sbStatus_Ok = 0,
  sbStatus_ErrorMustCallFixUp,
  sbStatus_ErrorNodeNotFound,
  sbStatus_ErrorFormat,
};

template< typename T >
struct sbStatusResult
{
  sbStatusResult( T result )
  : m_Result( result )
  , m_Status( sbStatus_Ok )
  {}
  
  sbStatusResult( T result, sbStatus status )
  : m_Result( result )
  , m_Status( status )
  {}
  
  T         m_Result;
  sbStatus  m_Status;
};

#endif
