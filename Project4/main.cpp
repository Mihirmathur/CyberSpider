//  Cyber Spider
//  CS32 - Project4
//  Created by Mihir Mathur on 3/5/16.
//  Copyright © 2016 Mihir Mathur. All rights reserved.


#include <iostream>
#include "DiskMultiMap.h"
#include "MultiMapTuple.h"
#include "IntelWeb.h"
#include <string>
#include <vector>
#include "InteractionTuple.h"
int main(){
//        DiskMultiMap x;
//        x.createNew("myhashtable.dat",100);
//    x.insert("hmm.exe", "pfft.exe", "m52902");
//    x.insert("http://www.foo.com/bar", "pfft.exe", "m52902");
//    x.insert("http://www.foo.com/bar", "pfft.exe", "m10001");
//    x.insert("hmm.exe", "pas.exe", "m10601");
//    x.insert("blah.exe", "bletch.exe", "m0003");
//    x.erase("hmm.exe", "pas.exe", "m10601");
//    DiskMultiMap::Iterator it=x.search("http://www.foo.com/bar");
//    while(it.isValid()){
//        MultiMapTuple m=*it;
//        cout<<m.key<<" "<<m.value<<" "<<m.context<<" "<<endl;
//        ++it;
//    }
    
    std::vector<std::string> indicators;
    unsigned int minPrevalenceToBeGood;
    std::vector<std::string> badEntitiesFound;
    std::vector<InteractionTuple> badInteractions;
    indicators.push_back("a.exe");
    indicators.push_back("www.rare-malicious-website.com");
    
    IntelWeb i;
    i.createNew("my", 5000); //Doesn't work for 1000 and foobar.
    i.ingest("telemetry2.txt");
    int k=i.crawl(indicators, 500, badEntitiesFound, badInteractions);
    cerr<<k<<endl;
    InteractionTuple tup;
    for (int i=0; i<badInteractions.size(); i++) {
        tup=badInteractions[i];
        cerr<<tup.context<<" "<<tup.from<<" "<<tup.to<<" "<<endl;
    }
    for (int i=0; i<badEntitiesFound.size(); i++) {
        cerr<<badEntitiesFound[i]<<endl;
    }
    
}
