//
//  sbPointerValue.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/24/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbPointerValue_h
#define StructuredBinary_sbPointerValue_h

class sbPointerValue
{
public:

  sbPointerValue();
  
  static sbPointerValue Pointer( const char* p );
  
  const char* AsConstCharStar() const;
  char* AsCharStar() const;

private:

  const char* m_Value;
};

#endif
