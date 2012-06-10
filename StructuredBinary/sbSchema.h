//
//  sbSchema.h
//  StructuredBinary
//
//  Created by Ronald Pieket-Weeserik on 6/2/12.
//  Copyright 2012 It Should Just Work!™. All rights reserved.
//

#ifndef StructuredBinary_sbSchema_h
#define StructuredBinary_sbSchema_h

class sbNode;
class sbScalarValue;

class sbSchema
{
public:
  
  sbSchema()
  : m_EntryCount( 0 )
  {}

  const sbNode* FindNode( const char* name ) const;
  sbNode* FindNode( const char* name );
  void AddNode( const char* name, sbNode* node );
  
  void PrintNode( const char* node_name, const char* data ) const;

  void FixUp();

  void Convert( char* dst_data, const char* src_data, const sbSchema* src_schema, const char* name ) const;

private:

  struct Entry
  {
    const char* m_Name;
    sbNode*     m_Node;
  };

  static const int  kMaxEntries = 100;
  int               m_EntryCount;
  Entry             m_Entries[ kMaxEntries ];
};

#endif
