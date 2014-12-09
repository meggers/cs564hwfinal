#include "catalog.h"


RelCatalog::RelCatalog(Status &status) :
	 HeapFile(RELCATNAME, status)
{
// nothing should be needed here
}


const Status RelCatalog::getInfo(const string & relation, RelDesc &record)
{
  if (relation.empty())
    return BADCATPARM;

  Status status;
  Record rec;
  RID rid;  
  
  // Create the heapFileScan (piazza cid=328)
  HeapFileScan* heapFile = new HeapFileScan(RELCATNAME, status);
  if(status != OK) return status;

  // No clue on parameters here, but need to start a scan
  status = heapFile->startScan(0, 0, STRING, relation.c_str(), EQ);  
  if(status != OK) return status;
  
  // Store the first matching RID in rid
  status = heapFile->scanNext(rid); 
  if (status != OK) return status; 
  
  // Get the actual record data
  status = getRecord(rid, rec); 
  if (status != OK) return status;
  
  // Memcpy into record return param
  memcpy(&record, &rec.data, rec.length);
}


const Status RelCatalog::addInfo(RelDesc & record)
{
  RID rid;
  InsertFileScan*  ifs;
  Status status;




}

const Status RelCatalog::removeInfo(const string & relation)
{
  Status status;
  RID rid;
  HeapFileScan*  hfs;

  if (relation.empty()) return BADCATPARM;



}


RelCatalog::~RelCatalog()
{
// nothing should be needed here
}


AttrCatalog::AttrCatalog(Status &status) :
	 HeapFile(ATTRCATNAME, status)
{
// nothing should be needed here
}


const Status AttrCatalog::getInfo(const string & relation, 
				  const string & attrName,
				  AttrDesc &record)
{

  Status status;
  RID rid;
  Record rec;
  HeapFileScan*  hfs;

  if (relation.empty() || attrName.empty()) return BADCATPARM;




}


const Status AttrCatalog::addInfo(AttrDesc & record)
{
  RID rid;
  InsertFileScan*  ifs;
  Status status;





}


const Status AttrCatalog::removeInfo(const string & relation, 
			       const string & attrName)
{
  Status status;
  Record rec;
  RID rid;
  AttrDesc record;
  HeapFileScan*  hfs;

  if (relation.empty() || attrName.empty()) return BADCATPARM;

}


const Status AttrCatalog::getRelInfo(const string & relation, 
				     int &attrCnt,
				     AttrDesc *&attrs)
{
  Status status;
  RID rid;
  Record rec;
  HeapFileScan*  hfs;

  if (relation.empty()) return BADCATPARM;




}


AttrCatalog::~AttrCatalog()
{
// nothing should be needed here
}

