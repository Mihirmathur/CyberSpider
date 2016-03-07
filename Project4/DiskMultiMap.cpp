#include "BinaryFile.h"
#include "DiskMultiMap.h"
#include <iostream>
#include <functional>

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
    NUM_BUCKETS=numBuckets;
    increaseOffset(sizeof(int));
    for (int i=0; i<numBuckets; i++) {
        bin.write(0, offset);
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
    if (sizeof(key)>120 || sizeof(value)>120 || sizeof(context)>120) {
        return 0;
    }
    
    Node n(key.c_str(), value.c_str(), context.c_str());
    std::hash<std::string> str_hash; // creates a string hasher.
    unsigned int hashValue = str_hash(n.key); // now hash the string.
    unsigned int bucket = hashValue % NUM_BUCKETS;
    n.offNext=-1;
    int t;
    bin.read(t, bucket+4);
    
    //If no node has been inserted into bucket, write the offset of node that
    //will be inserted into the bucket. (Bucket+4 because first 4 bytes contain NUM_BUCKETS)
    if (t==0) {
        bin.write(getOffset(), bucket+4);
    }
    //t!=0 implies that a node with that hash value is already stored at location 't'
    //So the offNext of that node has to be updated.
    else{
        Node temp;
        do{
            bin.read(temp, t);
            if (temp.offNext!=-1) {
                t=temp.offNext;
                //cerr<<temp.key<<" ";
            }
        }while (temp.offNext!=-1);
        
        //Update offNext of tempnode.
        temp.offNext=getOffset();
        //Re-write temp node at its position.
        bin.write(temp, t);
    }
    bin.write(n, getOffset());
    increaseOffset(sizeof(n));
    return true;
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key){
    
    std::hash<std::string> str_hash; // creates a string hasher.
    unsigned int hashValue = str_hash(key); // now hash the string.
    unsigned int bucket = hashValue % NUM_BUCKETS;
    int t;
    Node temp;
    bin.read(t, bucket+4);
    if(t!=0) {
        bin.read(temp, t);
        while(1){
            if (strcmp(temp.key, key.c_str())==0) {
                DiskMultiMap::Iterator it(1, t, this);
                return it;
            }
            if (temp.offNext==-1) {
                break;
            }
            else{
                bin.read(temp, temp.offNext);
            }
        }
        
//        do{
//            cout<<temp.key<<" "<<temp.value<<" "<<temp.context<<endl;
//            bin.read(temp, temp.offNext);
//        }while (temp.offNext!=-1);
    }
   
    DiskMultiMap::Iterator it(0, -1, this);
    return it;
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