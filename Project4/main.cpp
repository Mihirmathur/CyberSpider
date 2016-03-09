//  Cyber Spider
//  CS32 - Project4
//  Created by Mihir Mathur on 3/5/16.
//  Copyright © 2016 Mihir Mathur. All rights reserved.


#include <iostream>
#include "DiskMultiMap.h"
#include "MultiMapTuple.h"
int main(){
        DiskMultiMap x;
        x.createNew("myhashtable.dat",100);
    x.insert("hmm.exe", "pfft.exe", "m52902");
    x.insert("hmm.exe", "pfft.exe", "m52902");
    x.insert("hmm.exe", "pfft.exe", "m10001");
    x.insert("hmm.exe", "pas.exe", "m10601");
    x.insert("blah.exe", "bletch.exe", "m0003");
    x.erase("hmm.exe", "pas.exe", "m10601");
    DiskMultiMap::Iterator it=x.search("blah.exe");
    while(it.isValid()){
        MultiMapTuple m=*it;
        cout<<m.key<<" "<<m.value<<" "<<m.context<<" "<<endl;
        ++it;
    }
}
