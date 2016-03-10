//
//  IntelWeb.cpp
//  Project4


#include "BinaryFile.h"
#include "DiskMultiMap.h"
#include <iostream>
#include "IntelWeb.h"
#include "MultiMapTuple.h"
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
using namespace std;

//Helper functions for comparing Tuples
bool operator==(InteractionTuple a, InteractionTuple b){
    if (a.to==b.to && a.from==b.from && a.context==b.context) {
        return true;
    }
    return false;
}

bool compTuple(InteractionTuple a, InteractionTuple b){
    if (a.context<b.context) {
        return true;
    }
    else if(a.context==b.context){
        if(a.from<b.from){
            return true;
        }
        else if(a.from==b.from){
            if(a.to<b.to){
                return true;
            }
        }
    }
    return false;
}

//Helper function for checking if str is in set s.
bool IsInSet(string str, set<string> s){
    if(s.count(str)>0){
        return true;
    }
    return false;
}

IntelWeb::~IntelWeb(){
    m_diskMap.close();
    m_diskMap2.close();
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems){
    
    m_diskMap.close();
    m_diskMap2.close();
    file_prefix=filePrefix;
    bool y = m_diskMap.createNew(filePrefix + "-1.dat", floor(maxDataItems*1.3));
    if (!y) {
        m_diskMap.close();
        return false;
    }
    file_name1=filePrefix+"-1.dat";
    
    y= m_diskMap2.createNew(filePrefix+"-2.dat", floor(maxDataItems*1.3));
    if (!y) {
        m_diskMap.close();
        return false;
    }
    
    file_name2=filePrefix+"-2.dat";
    
    return true;
}

bool IntelWeb::openExisting(const std::string& filePrefix){
    m_diskMap.close();
    m_diskMap2.close();
    bool y=m_diskMap.openExisting(filePrefix + "-1.dat");
    if (!y) {
        m_diskMap.close();
        return false;
    }
    y= m_diskMap2.openExisting(filePrefix+"-2.dat");
    if (!y) {
        m_diskMap2.close();
        return false;
    }
    return true;
}

void IntelWeb::close(){
    m_diskMap.close();
    m_diskMap2.close();
}

bool IntelWeb::ingest(const std::string& telemetryFile){
    ifstream inf(telemetryFile);
    if (!inf){
        cerr << "Cannot open telemetry file." << endl;
        return false;
    }
    string line;
    string k,v,c;
    char dummy;
    while (getline(inf,line)) {
        istringstream iss(line);
        if ( ! (iss >> k >> v >> c) ){
            cerr << "Ignoring badly-formatted input line: " << line << endl;
            continue;
        }
        if (iss >> dummy);// succeeds if there a non-whitespace char
        //cerr<<k<<" "<<v<<" "<<c<<" "<<endl;
        m_diskMap.insert(v, c, k);
        m_diskMap2.insert(c, v, k);
    }
    return true;
}

unsigned int IntelWeb::crawl(
                   const std::vector<std::string>& indicators,
                   unsigned int minPrevalenceToBeGood,
                   std::vector<std::string>& badEntitiesFound,
                   std::vector<InteractionTuple>& badInteractions){
    
//    m_diskMap.openExisting(file_name1);
//    m_diskMap2.openExisting(file_name2);
    
    int num_mal=0;
    int count=0;
    badEntitiesFound.clear();
    InteractionTuple tuple;
    vector<string> indicators_ext=indicators;
    DiskMultiMap::Iterator it, iter, iter_2;
    set<std::string> mal_entity;
    for (int i=0; i<indicators_ext.size(); i++) {
        
        //Searching in first Map (If indicator is an initiator)
        it=m_diskMap.search(indicators_ext[i]);
        
        if (it.isValid() && !IsInSet(indicators_ext[i], mal_entity)) {
            num_mal++;
            mal_entity.insert(indicators_ext[i]);
            badEntitiesFound.push_back(indicators_ext[i]);
        }
        
        while(it.isValid()){
            MultiMapTuple m=*it;
            tuple.from=m.key;
            tuple.to=m.value;
            tuple.context=m.context;
//            cout<<m.key<<" "<<m.value<<" "<<m.context<<" "<<endl;
            //If this entity is not in the set already,
            //Search for it in map and map 2 to see if its prevalence is less than minPrevalence.
            //If it is less, insert it into badEntitiesFound
            if (!IsInSet(tuple.to, mal_entity)) {
                count=0;
                iter=m_diskMap2.search(tuple.to);
                while (iter.isValid()) {
                    count++;
                    ++iter;
                }
                iter=m_diskMap.search(tuple.to);
                
                while (iter.isValid()) {
                    count++;
                    ++iter;
                }
                
                if (count<minPrevalenceToBeGood) {
                    num_mal++;
                    mal_entity.insert(tuple.to);
                    indicators_ext.push_back(tuple.to);
                    badEntitiesFound.push_back(tuple.to);
                }
            }
            badInteractions.push_back(tuple);
            ++it;
        }
        
        //Searching in second map (if indicator is a target)
        iter_2=m_diskMap2.search(indicators_ext[i]);
        
        if (iter_2.isValid() && !IsInSet(indicators_ext[i], mal_entity)) {
            num_mal++;
            mal_entity.insert(indicators_ext[i]);
            badEntitiesFound.push_back(indicators_ext[i]);
        }
        
        while (iter_2.isValid()) {
            MultiMapTuple M=*iter_2;
            tuple.from=M.value;
            tuple.to=M.key;
            tuple.context=M.context;
            if (!IsInSet(tuple.from, mal_entity)) {
                count=0;
                iter=m_diskMap2.search(tuple.from);
                while (iter.isValid()) {
                    count++;
                    ++iter;
                }
                iter=m_diskMap.search(tuple.from);
                
                while (iter.isValid()) {
                    count++;
                    ++iter;
                }
                
                if (count<minPrevalenceToBeGood) {
                    num_mal++;
                    mal_entity.insert(tuple.from);
                    indicators_ext.push_back(tuple.from);
                    badEntitiesFound.push_back(tuple.from);
                }
                
            }
            badInteractions.push_back(tuple);
            ++iter_2;
        }
        
        
    }
    
    //Sorting and deleting repeat entries.
    sort( badInteractions.begin(), badInteractions.end(), compTuple );
    sort(badEntitiesFound.begin(), badEntitiesFound.end());
    badInteractions.erase( unique( badInteractions.begin(), badInteractions.end() ), badInteractions.end() );
    

    return num_mal;
}