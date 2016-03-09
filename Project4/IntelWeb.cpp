//
//  IntelWeb.cpp
//  Project4


#include "BinaryFile.h"
#include "DiskMultiMap.h"
#include <iostream>
#include "IntelWeb.h"
#include "MultiMapTuple.h"
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>
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
        m_diskMap.insert(k, v, c);
        
    }
    return true;
}