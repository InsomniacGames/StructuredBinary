//
//  sbField.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/27/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbField_h
#define StructuredBinary_sbField_h

class sbStruct;
class sbScalar;
class sbPointer;

class sbField
{
public:
  virtual ~sbField() {}
  virtual const sbScalar*   AsScalar()  const { return NULL; }
  virtual const sbStruct*   AsStruct()  const { return NULL; }
  virtual const sbPointer*  AsPointer() const { return NULL; }
};

#endif
