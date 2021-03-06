#include <sys/types.h>
#include <functional>
#include <string.h>
#include <stdio.h>
using namespace std;

#include "error.h"
#include "utility.h"
#include "catalog.h"

// define if debug output wanted

// Retrieves and prints information from the catalogs about the for
// the user. If no relation is given (relation.empty() is true), then
// it lists all the relations in the database, along with the width in
// bytes of the relation, the number of attributes in the relation,
// and the number of attributes that are indexed.  If a relation is
// given, then it lists all of the attributes of the relation, as well
// as its type, length, and offset
//
// Returns:
// 	OK on success
// 	error code otherwise
//

const Status RelCatalog::help(const string & relation)
{
  Status status;
  RelDesc rd;
  AttrDesc *attrs;
  int attrCnt;

  if (relation.empty()) return UT_Print(RELCATNAME);
  else
  {
    status = attrCat->getRelInfo(relation, attrCnt, attrs); // Get the relations to print
    if (status != OK) return status;
    
    cout << "Relation Name: " << relation << "(" << attrCnt << " attributes)" << endl;
    
    for(int i = 0; i < attrCnt; i++)
    {
      // Print em out here
      cout << attrs[i].attrName << attrs[i].attrOffset << attrs[i].attrType << attrs[i].attrLen << endl;
    }
  }

  return OK;
}
