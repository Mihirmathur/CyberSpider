#include "BinaryFile.h"
#include "DiskMultiMap.h"
#include <iostream>

using namespace std;

DiskMultiMap::DiskMultiMap(){
    offset=0;
}

DiskMultiMap::~DiskMultiMap(){
    bin.close();
}

bool DiskMultiMap::createNew(const std::string &filename, unsigned int numBuckets){
    bin.close();
    bool suc = bin.createNew(filename);
    if(!suc)return 0;
    bin.write(numBuckets, offset);
    increaseOffset(sizeof(int));
    for (int i=0; i<numBuckets; i++) {
        bin.write(i, offset);
        increaseOffset(sizeof(i));
    }
    return 1;
}

bool DiskMultiMap::openExisting(const std::string &filename){
    bin.close();
    bool suc;
    suc=bin.openExisting(filename);
    return suc;
}

void DiskMultiMap::close(){
    bin.close();
}

bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context){
    return true;
}
/*
class DiskList
{
public:
    DiskList(const std::string& filename);
    bool push_front(const char* data);
    bool remove(const char* data);
    void printAll();
private:
    BinaryFile bin;
    int off;
    string name;
};
DiskList::DiskList(const std::string& filename){
    bin.createNew(filename);
    name=filename;
    off=0;
}

bool DiskList::push_front(const char* data){
    bool success=bin.write(data,strlen(data), off);
    if(!success){
        cerr<<"Error";
        return false;
    }
    else {
        off=strlen(data);
        return true;
    }
}

void DiskList::printAll(){
    int i=0;
    char* t;
    bool y;
    cout<<bin.fileLength();
    bin.read(t, 4,i);
    i+=strlen(t);
    cout<<t;
    
}
*/