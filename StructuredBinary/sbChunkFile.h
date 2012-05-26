//
//  sbChunkFile.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbChunkFile_h
#define StructuredBinary_sbChunkFile_h

#include "sbChunk.h"

int sbChunkWrite( const char* file_name, const sbChunk* chunk );
const sbChunk* sbChunkRead( const char* file_name, char* buffer, int buffer_size );

#endif
