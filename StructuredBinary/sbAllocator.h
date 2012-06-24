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

class sbAllocator
{
public:
  
  struct Result
  {
    char* m_Data;
    bool  m_Done;
  };

  sbAllocator();
  sbAllocator( char* data, size_t size );
  void SetBuffer( char* data, size_t size );

  Result Alloc( size_t element_size, size_t element_count, size_t alignment, const char* src_data );
  size_t GetSize() const { return m_Offset; }
  void StorePointerLocation( const char* data );
  
  int GetPointerLocationCount() const;
  const char* GetPointerLocation( int index ) const;

private:
  static const int  kMaxEntries = 100;

  struct Entry
  {
    const char* m_SrcData;
    size_t      m_ElementSize;
    size_t      m_ElementCount;
    size_t      m_Alignment;
    size_t      m_Offset;
  };
  
  Entry   m_Entries[ kMaxEntries ];
  int     m_EntryCount;
  
  const char* m_Pointers[ kMaxEntries ];
  int         m_PointerCount;
  
  char*   m_Data;
  size_t  m_Offset;
  size_t  m_Size;

  Entry* FindEntry( size_t element_size, size_t element_count, size_t alignment, const char* src_data );
  Entry* AddEntry( size_t element_size, size_t element_count, size_t alignment, const char* src_data );
};

#endif
