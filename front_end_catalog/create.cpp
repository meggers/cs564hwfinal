#include "catalog.h"


const Status RelCatalog::createRel(const string & relation, 
				   const int attrCnt,
				   const attrInfo attrList[])
{
  Status status;
  RelDesc rd;
  AttrDesc ad;
  int offset;

  if (relation.empty() || attrCnt < 1)
    return BADCATPARM;

  if (relation.length() >= sizeof rd.relName)
    return NAMETOOLONG;

  if(getInfo(relation, rd) != OK) // If this table doesn't already exist
  {
    // Fill out new RelDesc
    strcpy(rd.relName, relation.c_str());
    rd.attrCnt = attrCnt;
    relCat->addInfo(rd); // Add to the catalog
   
    offset = 0; // Start at the beginning
    
    for(int i = 0; i < (sizeof(attrList)/sizeof(*attrList)); i++)
    {
      // Fill in AttrDesc instance
      strcpy(ad.relName, attrList[i].relName);
      strcpy(ad.attrName, attrList[i].attrName);
      ad.attrOffset = offset; // piazza cid = 320
      ad.attrType = attrList[i].attrType;
      ad.attrLen = attrList[i].attrLen;
      
      attrCat->addInfo(ad); // Add to the attribute catalog
      
      offset += attrList[i].attrLen; // Move the insertion point forward
    }
    
    status = createHeapFile(relation); // Create a heap file
    if (status != OK) return status;
    
    return OK;    
  }
}

