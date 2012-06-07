//
//  UnitTest.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/13/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#include "UnitTest.h"

#include <stdio.h>
#include <stdlib.h>
#include <new>

#include "UnitTestResult.h"

struct AllocationRecord
{
  void*   m_Pointer;
  size_t  m_Size;
};

AllocationRecord m_Records[ 10000 ];
int AllocationCount;

static AllocationRecord* FindFreeAllocationRecord()
{
  AllocationRecord* record = m_Records;
  AllocationRecord* end = m_Records + AllocationCount;
  while( record < end )
  {
    if( !record->m_Pointer )
    {
      return record;
    }
    record += 1;
  }
  
  AllocationCount += 1;
  return record;
}

static AllocationRecord* FindAllocationRecord( void* p )
{
  AllocationRecord* record = m_Records;
  AllocationRecord* end = m_Records + AllocationCount;
  while( record < end )
  {
    if( record->m_Pointer == p )
    {
      return record;
    }
    record += 1;
  }

  return NULL;
}

static void StoreAllocation( void* p, size_t size )
{
  AllocationRecord* record = FindFreeAllocationRecord();
  if( record )
  {
    record->m_Pointer = p;
    record->m_Size = size;
  }
}

static void FreeAllocation( void* p )
{
  AllocationRecord* record = FindAllocationRecord( p );
  if( record )
  {
    record->m_Pointer = NULL;
  }
  else
  {
    printf( "Could not free %p\n", p );
  }
}

static int CountRemainingAllocations()
{
  int count = 0;
  AllocationRecord* record = m_Records;
  AllocationRecord* end = m_Records + AllocationCount;
  while( record < end )
  {
    if( record->m_Pointer )
    {
      printf( "Leaking %lu bytes at %p\n", record->m_Size, record->m_Pointer );
      count += 1;
    }
    record += 1;
  }
  return count;
}


UnitTestResult UnitTest::Run() const
{
  AllocationCount = 0;
  UnitTestResult result = RunTest();
  if( result.IsSuccess() )
  {
    int count = CountRemainingAllocations();
    if( count > 0 )
    {
      return UnitTestResult::Error( "Leak count: %d\n", count );
    }
  }
  return result;
}

void* operator new( size_t size ) throw( std::bad_alloc );
void operator delete( void* p ) throw();

void* operator new( size_t size ) throw( std::bad_alloc )
{
  void* p = malloc( size );
  StoreAllocation( p, size );
  return p;
}

void* operator new[]( size_t size ) throw( std::bad_alloc )
{
  void* p = malloc( size );
  StoreAllocation( p, size );
  return p;
}

void operator delete( void* p ) throw()
{
  FreeAllocation( p );
  free( p );
}

void operator delete[]( void* p ) throw()
{
  FreeAllocation( p );
  free( p );
}
