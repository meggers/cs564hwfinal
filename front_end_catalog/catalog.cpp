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

  // Not sure of parameters here
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
  return OK;
}


const Status RelCatalog::addInfo(RelDesc & record)
{
  RID rid;
  InsertFileScan*  ifs;
  Record rec;
  Status status;

  // Create InsertFileScan on the relation catalog
  ifs = new InsertFileScan(RELCATNAME, status);
  if (status != OK) return status;
  
  // Create the record
  rec.data = &record;
  rec.length = sizeof(record);
  
  // Add this relation to relCat
  ifs->insertRecord(rec, rid);
}

const Status RelCatalog::removeInfo(const string & relation)
{
  Status status;
  RID rid;
  HeapFileScan*  hfs;

  if (relation.empty()) return BADCATPARM;
  
  // Create a scan on the relation catalog
  hfs = new HeapFileScan(RELCATNAME, status);
  if (status != OK) return status;
  
  // Start the scan, not sure on parameters here
  status = hfs->startScan(0, 0, STRING, relation.c_str(), EQ);
  if (status != OK) return status;
  
  // Get RID of desire relation within relCat
  status = hfs->scanNext(rid);
  if (status != OK) return status;
  
  status = hfs->deleteRecord(); // The curRec is deleted, which is set by scanNext
  if (status != OK) return status;
  
  return OK;	
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

	// Initialize Heap File Scan
	hfs = new HeapFileScan(ATTRCATNAME, status);
	if (status != OK) return status;

	// Start Scan
	status = hfs->startScan(0, sizeof(relation), STRING, relation.c_str(), EQ);
	if (status != OK) return status;

	while (status != FILEEOF) {
		status = hfs->scanNext(rid);
		if (status != OK) break;

		status = hfs->getRecord(rec);
		if (status != OK) break;

		memcpy(&record, rec.data, rec.length);
		if (strcmp(record.relName, relation.c_str()) == 0 && strcmp(record.attrName, attrName.c_str()) == 0) break;
	}

	status = hfs->endScan();
	delete hfs;
	return status;
}


const Status AttrCatalog::addInfo(AttrDesc & record)
{
	RID rid;
	Record rec;
	Status status;
	InsertFileScan*  ifs;

	ifs = new InsertFileScan(ATTRCATNAME, status);
	if (status != OK) return status;

	rec.data = &record;
	rec.length = sizeof(AttrDesc);

	status = ifs->insertRecord(rec, rid);
	return status;
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

	hfs = new HeapFileScan(ATTRCATNAME, status);
	if (status != OK) return status;

	status = hfs->startScan(0, relation.length() + 1, STRING, relation.c_str(), EQ);
	if (status != OK) return status;

	while (hfs->scanNext(rid) != FILEEOF) {
		status = hfs->getRecord(rec);
		if (status != OK) return status;

    		memcpy(&record, rec.data, rec.length);

		if (strcmp(record.relName, relation.c_str()) == 0 && strcmp(record.attrName, attrName.c_str()) == 0) {
			status = hfs->deleteRecord();
			hfs->endScan();
			delete hfs;
			return status;
		}
	}

	hfs->endScan();
	delete hfs;
	return ATTRNOTFOUND;
}

//Gotta do more on this
const Status AttrCatalog::getRelInfo(const string & relation, 
				     int &attrCnt,
				     AttrDesc *&attrs)
{
	Status status;
	RID rid;
	Record rec;
	HeapFileScan*  hfs;

	if (relation.empty()) return BADCATPARM;

	// Get information

	// Initialize Heap File Scan
	hfs = new HeapFileScan(ATTRCATNAME, status);
	if (status != OK) return status;

	// Start Scan
	status = hfs->startScan(0, sizeof(relation), STRING, relation.c_str(), EQ);
	if (status != OK) return status;

	while (status != FILEEOF) {
		status = hfs->scanNext(rid);
		if (status != OK) break;

		status = hfs->getRecord(rec);
		if (status != OK) break;

		attrCnt ++;
    		memcpy(&attrs[attrCnt - 1], rec.data, rec.length);
	}
	
	if (attrCnt == 0) return RELNOTFOUND;

	status = hfs->endScan();
	delete hfs;
	return status;
}


AttrCatalog::~AttrCatalog()
{
// nothing should be needed here
}

