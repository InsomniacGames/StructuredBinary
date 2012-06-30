//
//  UnitTest.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_UnitTest_h
#define StructuredBinary_UnitTest_h

#include <stdarg.h>

class UnitTest
{
public:

  class Result;

  virtual const char* GetName() const = 0;
  virtual Result RunTest() const = 0;
  Result Run() const;

  static Result Error( const char* message, ... );
  static Result Ok();
  
  class Result
  {
  public:
    Result( bool success, const char* text );
    
    bool IsSuccess() const { return m_Success; }
    const char* GetMessage() const { return m_String; }
    
    Result( const Result& other );
    Result& operator=( const Result& other );
    ~Result();
    
  private:    
    char* m_String;
    bool  m_Success;
  };
};

#endif
