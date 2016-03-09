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
using namespace std;

IntelWeb::~IntelWeb(){
    m_diskMap.close();
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems){
    m_diskMap.close();
    bool y = m_diskMap.createNew(filePrefix + "-all.dat", floor(maxDataItems*1.3));
    if (!y) {
        m_diskMap.close();
        return false;
    }
    file_name=filePrefix+"-all.dat";
    return true;
}

bool IntelWeb::openExisting(const std::string& filePrefix){
    m_diskMap.close();
    bool y=m_diskMap.openExisting(filePrefix + "-all.dat");
    if (!y) {
        m_diskMap.close();
        return false;
    }
    return true;
}

void IntelWeb::close(){
    m_diskMap.close();
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
    }
    return true;
}

unsigned int IntelWeb::crawl(
                   const std::vector<std::string>& indicators,
                   unsigned int minPrevalenceToBeGood,
                   std::vector<std::string>& badEntitiesFound,
                   std::vector<InteractionTuple>& badInteractions){
    m_diskMap.openExisting(file_name);
    
    int num_mal=0;
    badEntitiesFound.clear();
    InteractionTuple tuple;
    DiskMultiMap::Iterator it;
    for (int i=0; i<indicators.size(); i++) {
        it=m_diskMap.search(indicators[i]);
        while(it.isValid()){
            MultiMapTuple m=*it;
            tuple.from=m.key;
            tuple.to=m.value;
            tuple.context=m.context;
//            cout<<m.key<<" "<<m.value<<" "<<m.context<<" "<<endl;
            badInteractions.push_back(tuple);
            ++it;
        }
    }
//    sort( badInteractions.begin(), badInteractions.end() );
//    badInteractions.erase( unique( badInteractions.begin(), badInteractions.end() ), badInteractions.end() );
    
    //TODO: Return number of malicious entities.
    return num_mal;
}