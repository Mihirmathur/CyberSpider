#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "BinaryFile.h"
#include <string>
class DiskMultiMap
{
public:
    
    class Iterator
    {
    public:
        Iterator(){
            state=0;
            o=0;
        }
        
        // You may add additional constructors
        Iterator(int i, BinaryFile::Offset off, DiskMultiMap* ptr){
            state=i;
            o=off;
            m_diskptr=ptr;
        }
        
        bool isValid() const{return state;};
        
        Iterator& operator++(){
            if (!isValid()) {
                return *this;
            }
            BinaryFile *b=&(m_diskptr->bin);
            b->read(m_diskptr->N, o);
            
            if (m_diskptr->N.offNext==-1) {
                state=0;
            }
            else {
                b->read(m_diskptr->M, m_diskptr->N.offNext);
                char temp_key[121];
                strcpy(temp_key, m_diskptr->N.key);
                if (m_diskptr->M.deleted==true || strcmp(m_diskptr->M.key, temp_key)!=0) {
                    state = 0;
                }
                else setO(m_diskptr->N.offNext);
            }
            
            return *this;
        };
        
        MultiMapTuple operator*(){
            MultiMapTuple m_tuple;
            if (!isValid()) {
                m_tuple.key="";
                m_tuple.value="";
                m_tuple.context="";
            }
            BinaryFile *b=&(m_diskptr->bin);
            b->read(m_diskptr->N, o);
            m_tuple.key=m_diskptr->N.key;
            m_tuple.value=m_diskptr->N.value;
            m_tuple.context=m_diskptr->N.context;
            return m_tuple;
        }
        
    private:
        // Your private member declarations will go here
        //0 - invalid, 1 - valid.
        int state;
        BinaryFile::Offset o;
        DiskMultiMap* m_diskptr;
        void setState(int y){state=y;}
        void setO(BinaryFile::Offset off){
            o=off;
        }
        
    };
    
    DiskMultiMap();
    ~DiskMultiMap();
    bool createNew(const std::string& filename, unsigned int numBuckets);
    bool openExisting(const std::string& filename);
    void close();
    bool insert(const std::string& key, const std::string& value, const std::string& context);
    Iterator search(const std::string& key);
    int erase(const std::string& key, const std::string& value, const std::string& context);
    
private:
    // Your private member declarations will go here
    struct Node{
        //MultiMapTuple map;
        Node(){deleted = false;}
        Node (const char k[], const char v[], const char c[]){
            strcpy(key, k);
            strcpy(value, v);
            strcpy(context, c);
            deleted = false;
        }
        char key[121];
        char value[121];
        char context[121];
        BinaryFile::Offset offNext;
        bool deleted;
    };
    void increaseOffset(long t){offset+=t;}
    BinaryFile::Offset getOffset(){return offset;}
    void setOffset(BinaryFile::Offset t){offset=t;}
    BinaryFile bin;
    BinaryFile::Offset offset; //Points to the end of file. Used for writing new things. 
    BinaryFile::Offset head_offset;//Points to the start of the nodes.
    Node N, M; // Nodes for working.
    void Display();
    int NUM_BUCKETS;
};

#endif // DISKMULTIMAP_H_