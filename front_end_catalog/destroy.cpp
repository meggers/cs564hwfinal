#include "catalog.h"

//
// Destroys a relation. It performs the following steps:
//
// 	removes the catalog entry for the relation
// 	destroys the heap file containing the tuples in the relation
//
// Returns:
// 	OK on success
// 	error code otherwise
//

const Status RelCatalog::destroyRel(const string & relation)
{
  Status status;
  RelDesc rd;

  if (relation.empty() || 
      relation == string(RELCATNAME) || 
      relation == string(ATTRCATNAME))
    return BADCATPARM;

  if(getInfo(relation, rd) == OK) // There is a relation to be destroyed
  {
    // Clean up meta-data
    attrCat->dropRelation(relation);
    relCat->removeInfo(relation);
    
    // Destroy the file
    destroyHeapFile(relation);
  }
  
  return OK;
}


//
// Drops a relation. It performs the following steps:
//
// 	removes the catalog entries for the relation
//
// Returns:
// 	OK on success
// 	error code otherwise
//

const Status AttrCatalog::dropRelation(const string & relation)
{
	Status status;
	AttrDesc *attrs;
	int attrCnt, i;

	if (relation.empty()) return BADCATPARM;

	status = getRelInfo(relation, attrCnt, attrs); 
	if (status != OK) return status;

	for(i = 0; i < attrCnt; i++) {
		status = removeInfo(relation, attrs[i].attrName);
		if (status != OK) return status;
	}

	return OK;
}


