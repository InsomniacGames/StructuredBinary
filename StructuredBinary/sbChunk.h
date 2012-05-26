//
//  sbChunk.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbChunk_h
#define StructuredBinary_sbChunk_h

#include <cstring>
#include <stdio.h>
#include <stdint.h>

#define CHUNK_ID( a, b, c, d ) ((((uint32_t)a)<<0)|(((uint32_t)b)<<8)|(((uint32_t)c)<<16)|(((uint32_t)d)<<24))

class sbChunk
{
public:
  sbChunk( uint32_t id )
  : m_Id( id )
  , m_DataSize( 0 )
  , m_Data( NULL )
  , m_Sibling( NULL )
  , m_Child( NULL )
  , m_ChildCount( 0 )
  {}
  
  sbChunk( uint32_t id, const void* data, uint32_t data_size )
  : m_Id( id )
  , m_DataSize( data_size )
  , m_Data( data )
  , m_Sibling( NULL )
  , m_Child( NULL )
  , m_ChildCount( 0 )
  {}

  ~sbChunk();
  
  const void* GetData() const { return m_Data; }
  const uint32_t GetDataSize() const { return m_DataSize; }
  const uint32_t GetId() const { return m_Id; }
  const sbChunk* GetChild() const { return m_Child; }
  const sbChunk* GetSibling() const { return m_Sibling; }
  int GetChildCount() const { return m_ChildCount; }

  sbChunk* AddChild( sbChunk* chunk );
  
private:
  
  uint32_t    m_Id;
  uint32_t    m_DataSize;
  const void* m_Data;
  sbChunk*      m_Sibling;
  sbChunk*      m_Child;
  int         m_ChildCount;
};


#endif
