//
//  UnitTestChunk.cpp
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 5/16/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

// Self
#include "UnitTestChunk.h"

// Libraries
#include <cstring>
#include <stdio.h>
#include <stdint.h>

// Project
#include "Number.h"
#include "Field.h"
#include "Aggregate.h"
#include "Fnv.h"

struct ChunkHeader
{
  uint32_t  m_Name;
  uint32_t  m_Size;
};

class ChunkReadFile
{
public:
  ChunkReadFile()
  : m_File( NULL )
  {}
  
  bool Open( const char* file_name );
  uint32_t GetName() const { return m_Header.m_Name; }
  uint32_t GetSize() const { return m_Header.m_Size; }
  
private:
  FILE*       m_File;
  ChunkHeader m_Header;
};

bool ChunkReadFile::Open( const char* file_name )
{
  m_File = fopen( file_name, "rb" );
  if( m_File )
  {
    fread( &m_Header, 1, sizeof( m_Header ), m_File );
  }
  return m_File != NULL;
}

class ChunkWriteFile
{
public:
  ChunkWriteFile()
  : m_File( NULL )
  {}
  
  bool Open( const char* file_name );
  void Write( const char* data, uint32_t size );
  void Close();
  void SetName( uint32_t name ) { m_Header.m_Name = name; }
  
private:
  FILE*       m_File;
  ChunkHeader m_Header;
};

bool ChunkWriteFile::Open( const char* file_name )
{
  m_File = fopen( file_name, "wb" );
  if( m_File )
  {
    ChunkHeader dummy_header;
    dummy_header.m_Name = 0;
    dummy_header.m_Size = 0;
    fwrite( &dummy_header, 1, sizeof( dummy_header ), m_File );
  }
  return m_File != NULL;
}

void ChunkWriteFile::Write( const char* data, uint32_t size )
{
  fwrite( data, 1, size, m_File );
}

void ChunkWriteFile::Close()
{
  size_t size = ftell( m_File );
  m_Header.m_Size = ( uint32_t )size;
  fseek( m_File, 0, SEEK_SET );
  fwrite( &m_Header, 1, sizeof( m_Header ), m_File );
}


static const char* s_TestFileName = "RonsTestFile.chunk";

const char* UnitTestChunk::RunTest() const
{
  ChunkWriteFile wf;
  if( wf.Open( s_TestFileName ) )
  {
    wf.SetName( Fnv32( "Header" ) );
    wf.Write( "ABCDEFGH", 8 );
    wf.Close();
  }
  else
    return "Could not open file";

  return NULL;
}
