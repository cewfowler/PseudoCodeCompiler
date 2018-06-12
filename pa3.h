//
//  Header.h
//  pa3
//
//  Created by Christian on 11/17/16.
//  Copyright © 2016 Christian. All rights reserved.
//

#ifndef Header_h
#define Header_h

class NumberOf {
private:
    //holds point to add next string and declares array of size 100
    int currentPoint = 0;
    std::string number[100] = {};
    
public:
    void addToArray(std::string x);
    void print();
};

#endif /* Header_h */
