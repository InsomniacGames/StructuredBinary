//
//  sbAllocator.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/10/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbAllocator_h
#define StructuredBinary_sbAllocator_h

#include <stddef.h>


class sbType;
struct sbBlock
{
  sbBlock( const sbType* dst_type, const sbType* src_type, const char* src_data, int count )
  : m_DstType( dst_type )
  , m_SrcType( src_type )
  , m_SrcData( src_data )
  , m_Count( count )
  {}
  
  sbBlock()
  : m_DstType( NULL )
  , m_SrcType( NULL )
  , m_SrcData( NULL )
  , m_Count( 0 )
  {}
  
  bool operator == ( const sbBlock& other )
  {
    return  m_DstType == other.m_DstType &&
            m_SrcType == other.m_SrcType &&
            m_SrcData == other.m_SrcData &&
            m_Count   == other.m_Count;
  }

  const sbType* m_DstType;
  const sbType* m_SrcType;
  const char*   m_SrcData;
  int           m_Count;
};

class sbAllocator
{
public:
  
  sbAllocator();
  sbAllocator( char* data, size_t size );
  void SetBuffer( char* data, size_t size );

  char* Alloc( const sbType* dst_type, const sbType* src_type, const char* src_data, int count );
  size_t GetSize() const { return m_Offset; }
  void StorePointerLocation( const char* data );
  
  int GetPointerLocationCount() const;
  const char* GetPointerLocation( int index ) const;
  
  int GetCount() const { return m_EntryCount; }
  char* GetDstPtr( int index ) const
  {
    if( !m_Data ) return NULL;
    if( m_Offset > m_Size ) return NULL;
    return m_Data + m_Entries[ index ].m_Offset;
  }
  sbBlock GetBlock( int index ) const
  {
    return m_Entries[ index ].m_Block;
  }

private:
  static const int  kMaxEntries = 100;

  struct Entry
  {
    sbBlock     m_Block;
    size_t      m_Offset;
  };
  
  Entry   m_Entries[ kMaxEntries ];
  int     m_EntryCount;
  
  const char* m_Pointers[ kMaxEntries ];
  int         m_PointerCount;
  
  char*   m_Data;
  size_t  m_Offset;
  size_t  m_Size;

  Entry* FindEntry( const sbBlock& block );
  Entry* AddEntry( const sbBlock& block );
};

#endif
