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

	hfs = new HeapFileScan(RELCATNAME, status);
	if (status != OK) return status;

	status = hfs->startScan(0, relation.length() + 1, STRING, relation, EQ);
	if (status != OK) return status;

	status = hfs->scanNext(rid);
	if (status == FILEEOF) status = RELNOTFOUND;
	
	status = hfs->deleteRecord();
	
	hfs->endScan();
	return status;
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
	status = hfs-startScan(0, sizeof(relation), STRING, relation, EQ);
	if (status != OK) return status;

	while (status != FILEEOF) {
		status = hfs->scanNext(rid);
		if (status != OK) break;

		status = hfs->getRecord(rec);
		if (status != OK) break;

		memcpy(&record, rec.data, rec.length);
		if(strcmp(record.relName, relation) == 0 && strcmp(record.attrName, attrName) == 0) break;
	}

	status = hfs->endScan();
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

	// Initialize Heap File Scan
	hfs = new HeapFileScan(ATTRCATNAME, status);
	if (status != OK) return status;

	// Start Scan
	status = hfs-startScan(0, sizeof(relation), STRING, relation, EQ);
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
	return status;
}


AttrCatalog::~AttrCatalog()
{
// nothing should be needed here
}

