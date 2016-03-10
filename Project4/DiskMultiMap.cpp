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

void DiskMultiMap::Display(){
    BinaryFile::Offset q,j, off=8;
    cerr<<endl;
    for (j=0; j<NUM_BUCKETS; j++) {
        bin.read(q, off);
        off+=sizeof(BinaryFile::Offset);
        cerr<<q<<" ";
    }
}

bool DiskMultiMap::createNew(const std::string &filename, unsigned int numBuckets){
    bin.close();
    bool suc = bin.createNew(filename);
    if(!suc)return 0;
    
    //First 4 bytes contain number of buckets.
    bin.write(numBuckets, offset);
    NUM_BUCKETS=numBuckets;
    increaseOffset(sizeof(int));
    
    //Next 4 bytes contain offset of first deleted node => initialized to -100 (arbitrary value)
    bin.write(-100, offset);
    increaseOffset(sizeof(int));
    
    for (BinaryFile::Offset i=0; i<numBuckets; i++) {
        bin.write(0, offset);
        increaseOffset(sizeof(BinaryFile::Offset));
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
    unsigned int hashValue = str_hash(key); // now hash the string.
    unsigned int bucket = hashValue % NUM_BUCKETS;
    n.offNext=-1;
    //cerr<<"\n\n"<<bucket;
    BinaryFile::Offset t,q;
    bin.read(t, sizeof(BinaryFile::Offset)*bucket+8);
    
    //If no node has been inserted into bucket, write the offset of node that
    //will be inserted into the bucket. (Bucket+8 because first 8 bytes contain header)
    if (t==0) {
        q=getOffset();
        bin.write(getOffset(),sizeof(BinaryFile::Offset)*bucket+8);
        //cerr<<q<<endl;
    }
    //t!=0 implies that a node with that hash value is already stored at location 't'
    //So the offNext of that node has to be updated.
    else{
        //cerr<<t<<endl;
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
    //Display();
    return true;
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key){
    
    std::hash<std::string> str_hash; // creates a string hasher.
    unsigned int hashValue = str_hash(key); // now hash the string.
    unsigned int bucket = hashValue % NUM_BUCKETS;
    BinaryFile::Offset t,o, p;
    Node temp;
    bin.read(t, sizeof(BinaryFile::Offset)*bucket+8);
    o=t;
    p=t;
    if(t!=0) {
        bin.read(temp, t);
        while(1){
            if (strcmp(temp.key, key.c_str())==0 && temp.deleted==false) {
                DiskMultiMap::Iterator it(1, p, this);
                return it;
            }
            if (temp.offNext==-1) {
                break;
            }
            else{
                p=temp.offNext;
                bin.read(temp, temp.offNext);
                o=temp.offNext;
            }
        }
        
    }
   
    DiskMultiMap::Iterator it(0, -1, this);
    return it;
}

int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context){
    
    std::hash<std::string> str_hash; // creates a string hasher.
    unsigned int hashValue = str_hash(key); // now hash the string.
    unsigned int bucket = hashValue % NUM_BUCKETS;
    
    Node temp, tempNext;
    int count =0;
    BinaryFile::Offset t, o;
    bin.read(t, sizeof(BinaryFile::Offset)*bucket+8);
    if (t==0)
        return 0;
    bin.read(temp, t);
    o=t;
    
    if (temp.offNext==-1) {
        if (temp.key==key && temp.value==value && temp.context==context && temp.deleted==false){
            temp.deleted=true;
            count++;
            bin.write(temp, t);
            bin.write(0, sizeof(BinaryFile::Offset)*bucket+8);
        }
        return count;
    }
    
    else{
        if (temp.key==key && temp.value==value && temp.context==context && temp.deleted==false){
            temp.deleted=true;
            count++;
            bin.write(temp, t);
            bin.write(temp.offNext, sizeof(BinaryFile::Offset)*bucket+8);
        }
    }
    
    do{
        bin.read(tempNext, temp.offNext);
        if (tempNext.key==key && tempNext.value==value && tempNext.context==context && tempNext.deleted==false) {
            tempNext.deleted=true;
            count++;
            bin.write(tempNext, temp.offNext);
            if (temp.deleted==false) {
                temp.offNext=tempNext.offNext;
                bin.write(temp, o);
            }
            else{
                //bin.write(temp.offNext, sizeof(BinaryFile::Offset)*bucket+8);
            }
        }
        o=temp.offNext;
        temp=tempNext;
        
    }while(temp.offNext!=-1);
   
    bool doit;
    do{
        doit=false;
    bin.read(t, sizeof(BinaryFile::Offset)*bucket+8);
    
    if(t==0)return count;
    bin.read(temp, t);
    o=t;
    while (temp.offNext!=-1) {
        bin.read(tempNext, temp.offNext);
        if (tempNext.deleted==true) {
            temp.offNext=tempNext.offNext;
            bin.write(temp, o);
            doit=true;
        }
        o=temp.offNext;
        temp=tempNext;
    }
    }while (doit==true);
    
//    else{
//    do{
//        if (temp.key==key && temp.value==value && temp.context==context && temp.deleted==false) {
//            count++;
//            temp.deleted=true;
//            bin.write(temp, o);
//        }
//        o=temp.offNext;
//        if (o!=-1) {
//           bin.read(temp, o);
//        }
//    }while(o!=-1);
//    }
    return count;
    
}
