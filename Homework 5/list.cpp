//
//  list.cpp
//  Homework 5
//
//  Created by Cody Do on 3/8/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

void listAll(string path, const Class* c)  // two-parameter overload
{
    //If nullptr return
    if (c == nullptr)
        return;
    
    //Add name to path
    string name = c->name();
    if (path.empty())
        path = name;
    else
        path = path + "=>" + name;
    
    //Prints out path
    cout << path << endl;
    
    //Recursive call towards each subclass
    if (!c->subclasses().empty()) {
        vector<Class*>::const_iterator it = c->subclasses().begin();
        while (it != c->subclasses().end()) {
            listAll(path, *it);
            it++;
        }
    }
}

