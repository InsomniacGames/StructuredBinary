//
//  sbDictionary.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/17/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbDictionary_h
#define StructuredBinary_sbDictionary_h

#include "sbHash.h"

#include <cstring>

template< typename T >
class sbDictionary
{
public:
  sbDictionary( int initial_capacity, T not_found_value )
  : m_Name( NULL )
  , m_Payload( NULL )
  , m_NotFoundValue( not_found_value )
  , m_Capacity( 0 )
  , m_Count( 0 )
  {
    Resize( initial_capacity );
  }

  ~sbDictionary()
  {
    delete[] m_Name;
    delete[] m_Payload;
  }

  const T& FindByName( sbHash name ) const
  {
    for( int i = 0; i < m_Count; ++i )
    {
      if( m_Name[ i ] == name )
      {
        return m_Payload[ i ];
      }
    }
    return m_NotFoundValue;
  }
  
  const T& GetByIndex( int index ) const
  {
    if( index < m_Count )
    {
      return m_Payload[ index ];
    }
    return m_NotFoundValue;
  }
  
  sbHash GetNameByIndex( int index ) const
  {
    if( index < m_Count )
    {
      return m_Name[ index ];
    }
    return 0u;
  }
  
  void Add( sbHash name, T payload )
  {
    if( m_Count == m_Capacity )
    {
      Grow();
    }
    m_Name[ m_Count ] = name;
    m_Payload[ m_Count ] = payload;
    m_Count += 1;
  }

  int GetCount() const
  {
    return m_Count;
  }

private:
  sbHash* m_Name;
  T*      m_Payload;

  T       m_NotFoundValue;

  int     m_Capacity;
  int     m_Count;

  void Resize( int new_capacity )
  {
    T* old_payload = m_Payload;
    sbHash* old_name = m_Name;
    
    m_Capacity = new_capacity;
    m_Count = m_Capacity < m_Count ? m_Capacity : m_Count;
    
    m_Payload = m_Capacity ? new T[ m_Capacity ] : NULL;
    m_Name = m_Capacity ? new sbHash[ m_Capacity ] : NULL;
    
    for( int i = 0; i < m_Count; ++i )
    {
      m_Payload[ i ] = old_payload[ i ];
      m_Name[ i ] = old_name[ i ];
    }
    
    delete[] old_name;
    delete[] old_payload;
  }

  void Grow()
  {
    if( m_Capacity == 0 )
    {
      Resize( 1 );
    }
    else
    {
      Resize( m_Capacity * 2 );
    }
  }
};

#endif
