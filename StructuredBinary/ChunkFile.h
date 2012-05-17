//
//  ChunkFile.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_ChunkFile_h
#define StructuredBinary_ChunkFile_h

#include "Chunk.h"

bool ChunkWrite( const char* file_name, const Chunk* chunk );

#endif
