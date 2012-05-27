//
//  UnitTestArrayConvert.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/26/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "UnitTestArrayConvert.h"

// Libraries
#include <cstring>
#include <stdint.h>
#include <stdio.h>

// Project
#include "sbScalarValue.h"
#include "sbScalar.h"
#include "sbStruct.h"

//--------------------------------------------------------------------------------------------------

struct SourceStruct
{
  uint8_t   array1[ 3 ];
  uint8_t   array2[ 3 ];
};

struct DestStruct
{
  uint8_t   array2[ 3 ];
  uint8_t   array1[ 3 ];
};

//--------------------------------------------------------------------------------------------------

const char* UnitTestArrayConvert::RunTest() const
{
  sbStruct src_agg( 100, 100 );
  src_agg.AddScalar( sbFnv32( "array1" ), sbFieldType_ScalarU8, 3 );
  src_agg.AddScalar( sbFnv32( "array2" ), sbFieldType_ScalarU8, 3 );

  sbStruct dst_agg( 100, 100 );
  dst_agg.AddScalar( sbFnv32( "array2" ), sbFieldType_ScalarU8, 3 );
  dst_agg.AddScalar( sbFnv32( "array1" ), sbFieldType_ScalarU8, 3 );
  
  SourceStruct src;
  src.array1[ 0 ] = 0x79;
  src.array1[ 1 ] = 0x2a;
  src.array1[ 2 ] = 0xc9;
  src.array2[ 0 ] = 0xfe;
  src.array2[ 1 ] = 0x91;
  src.array2[ 2 ] = 0xac;
  
  DestStruct dst;
  
  const char* src_data = ( const char* )&src;
  char* dst_data = ( char* )&dst;
  
  dst_agg.Convert( dst_data, src_data, &src_agg );
  
  if( dst.array1[ 0 ] != src.array1[ 0 ] )  return "array1[ 0 ] converted incorrectly";
  if( dst.array1[ 1 ] != src.array1[ 1 ] )  return "array1[ 1 ] converted incorrectly";
  if( dst.array1[ 2 ] != src.array1[ 2 ] )  return "array1[ 2 ] converted incorrectly";
  if( dst.array2[ 0 ] != src.array2[ 0 ] )  return "array2[ 0 ] converted incorrectly";
  if( dst.array2[ 1 ] != src.array2[ 1 ] )  return "array2[ 1 ] converted incorrectly";
  if( dst.array2[ 2 ] != src.array2[ 2 ] )  return "array2[ 2 ] converted incorrectly";

  return NULL;
}
