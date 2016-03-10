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
//    x.insert( "http://www.foo.com/bar", "qqqqqa.exe", "m1");
//    x.insert( "qqqqqa.exe", "qqqqqb.exe", "m1");
//    x.insert( "qqqqqb.exe", "qqqqqc.exe", "m1");
//    x.insert( "qqqqqb.exe", "http://www.bar.com/bletch", "m1");
//    x.insert( "qqqqqd.exe", "http://www.bar.com/bletch", "m2");
//    x.insert( "qqqqqd.exe", "qqqqqf.exe", "m2");
//    x.insert( "qqqqqf.exe", "http://www.boo.edu/abc", "m2");
//    x.insert( "http://www.boo.edu/abc", "qqqqqg.exe", "m3");
//    x.insert( "qqqqqg.exe", "qqqqqh.exe", "m3");
//    x.insert( "explorer.exe", "g2.exe", "m4");
//    x.insert( "explorer.exe", "qqqqqa.exe", "m3");
//    x.insert( "explorer.exe", "g1.exe", "m4");
//    x.insert( "explorer.exe", "g2.exe", "m4");
//    x.insert( "explorer.exe", "g2.exe", "m4");;
//    x.insert( "explorer.exe", "g4.exe", "m5");
//    x.insert( "explorer.exe", "g5.exe", "m6");
//    x.insert( "explorer.exe", "g6.exe", "m6");
//    
//    x.insert( "firefox.exe", "http://google.com", "m7");
//    x.insert( "firefox.exe", "http://google.com", "m7");
//    x.insert( "explorer.exe", "http://google.com", "m7");
//    x.insert( "explorer.exe", "http://google.com", "m7");
//    x.insert( "explorer.exe", "http://google.com", "m7");
//    x.insert( "explorer.exe", "g2.exe", "m4");
//    x.insert( "explorer.exe", "g2.exe", "m4");
//    x.insert( "explorer.exe", "g2.exe", "m4");
//    int t=x.erase("explorer.exe", "g2.exe", "m4");
//    cerr<<t<<endl;
//    DiskMultiMap::Iterator it=x.search("explorer.exe");
//    while(it.isValid()){
//        MultiMapTuple m=*it;
//        cout<<m.key<<" "<<m.value<<" "<<m.context<<" "<<endl;
//        ++it;
//    }
    
    
    std::vector<std::string> indicators;
    unsigned int minPrevalenceToBeGood;
    std::vector<std::string> badEntitiesFound;
    std::vector<InteractionTuple> badInteractions;
    indicators.push_back("qqqqqa.exe");
    indicators.push_back("sbv.exe");
    
    IntelWeb i;
    i.createNew("myo", 80000); //Doesn't work for 1000 and foobar.
    i.ingest("output.txt");
    cerr<<"Completed Ingest.\n";
    i.purge("a.exe");
//    int k=i.crawl(indicators, 4, badEntitiesFound, badInteractions);
//    cerr<<"Malicious Entities: "<<k<<endl;
//    InteractionTuple tup;
//    for (int i=0; i<badInteractions.size(); i++) {
//        tup=badInteractions[i];
//        cerr<<tup.context<<" "<<tup.from<<" "<<tup.to<<" "<<endl;
//    }
//    for (int i=0; i<badEntitiesFound.size(); i++) {
//        cerr<<badEntitiesFound[i]<<endl;
//    }
  i.close();
}
