//
//  Util.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_Util_h
#define StructuredBinary_Util_h

#define ARRAY_SIZE( a ) ( int )( sizeof( a ) / sizeof( *a ) )

#define FIX_ALIGNMENT( value, alignment ) ( ( value ) + ( -( value ) & ( ( alignment ) - 1 ) ) )

#endif
