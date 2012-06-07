//
//  UnitTestResult.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/3/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "UnitTestResult.h"

#include <stdarg.h>
#include <string.h>
#include <cstdio>

UnitTestResult UnitTestResult::Error( const char* message, ... )
{
  char buffer[ 1000 ];
  va_list vlist;

  va_start( vlist, message );
  vsnprintf( buffer, sizeof( buffer ) - 1, message, vlist );
  buffer[ sizeof( buffer ) - 1 ] = '\0';
  va_end( vlist );

  return UnitTestResult( false, buffer );
}

UnitTestResult UnitTestResult::Ok()
{
  return UnitTestResult( true, NULL );
}

UnitTestResult::UnitTestResult( bool success, const char* text )
{
  m_String = NULL;
  if( text )
  {
    m_String = new char[ strlen( text ) + 1 ];
    strcpy( m_String, text );
  }
  m_Success = success;
}

UnitTestResult::~UnitTestResult()
{
  delete[] m_String;
}

UnitTestResult::UnitTestResult( const UnitTestResult& other )
{
  m_String = NULL;
  m_Success = other.m_Success;
  if( other.m_String )
  {
    m_String = new char[ strlen( other.m_String ) + 1 ];
    strcpy( m_String, other.m_String );
  }
}

UnitTestResult& UnitTestResult::operator=( const UnitTestResult& other )
{
  if (this != &other )
  {
    delete[] m_String;
    m_String = NULL;
    m_Success = other.m_Success;
    if( other.m_String )
    {
      m_String = new char[ strlen( other.m_String ) + 1 ];
      strcpy( m_String, other.m_String );
    }
  }
  return *this;
}


