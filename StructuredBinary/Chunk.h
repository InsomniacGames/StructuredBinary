//
//  Chunk.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_Chunk_h
#define StructuredBinary_Chunk_h

#include <cstring>
#include <stdio.h>
#include <stdint.h>

class Chunk
{
public:
  Chunk( const char* name )
  : m_Name( name )
  , m_DataSize( 0 )
  , m_Data( NULL )
  , m_Sibling( NULL )
  , m_Child( NULL )
  , m_ChildCount( 0 )
  {}
  
  ~Chunk();
  
  const void* GetData() const { return m_Data; }
  const uint32_t GetDataSize() const { return m_DataSize; }
  const char* GetName() const { return m_Name; }
  const Chunk* GetChild() const { return m_Child; }
  const Chunk* GetSibling() const { return m_Sibling; }
  int GetChildCount() const { return m_ChildCount; }
  
  Chunk* AddChunk( const char* name );
  void AddLeafChunk( const char* name, const void* data, uint32_t data_size );
  
private:
  Chunk( const char* name, const void* data, uint32_t data_size )
  : m_Name( name )
  , m_DataSize( data_size )
  , m_Data( data )
  , m_Sibling( NULL )
  , m_Child( NULL )
  , m_ChildCount( 0 )
  {}
  
  const char* m_Name;
  uint32_t    m_DataSize;
  const void* m_Data;
  Chunk*      m_Sibling;
  Chunk*      m_Child;
  int         m_ChildCount;
};


#endif
