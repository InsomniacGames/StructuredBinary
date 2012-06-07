//
//  UnitTestResult.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/3/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTestResult_h
#define StructuredBinary_UnitTestResult_h

#include <stdarg.h>

class UnitTestResult
{
public:
  static UnitTestResult Error( const char* message, ... );
  static UnitTestResult Ok();

  bool IsSuccess() const { return m_Success; }
  const char* GetMessage() const { return m_String; }

  UnitTestResult( const UnitTestResult& other );
  UnitTestResult& operator=( const UnitTestResult& other );
  ~UnitTestResult();

private:
  UnitTestResult( bool success, const char* text );

  char* m_String;
  bool  m_Success;
};

#endif
