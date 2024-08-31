//
// Created by Bibin Sibi
// Red Id: 825763777
// CS 480


#include "tree.h"
#include <bits/stdc++.h>

tree::tree(){
}
bool tree::insert(const char *stringBeingInserted){
    CharNode *currNode = rootNode;


    //interate through each letter
    for(char *x = (char*)stringBeingInserted; *x; x++){
        char c = (char)tolower(*x);
        //starts at root node and inserts chars down the tree


        int index = findIndex(c);
        //if that letter is not a valid character
         if(index == -1)
        {
             //dont do anything and skip
        }
        //if the current sequence of chars doesn't already exist, then u can insert
        else if(currNode->next[index] == nullptr){
            currNode->next[index] = new CharNode();
            currNode = currNode->next[index];

        }//otherwise just follow the path that already exists
        else{
            currNode = currNode->next[index];
        }
    }
    return false;
}
void tree::destroyTree(){
    //call helper method to deallocate all nodes in tree
    return deallocate(rootNode);
}

void tree::deallocate(CharNode *node) {
    //iterate through the branches of the tree
    for(int i = 0; i < 31 ; i++){
        //this will keep recursing as long as the node is not leaf node
            if(node->next[i]!=nullptr) {
                deallocate(node->next[i]);
            }
    }
    //once it hits the leaf nodes, they will get deallocated recursively up
        free(node);
}

bool tree::search(const char * strBeingSearched){
    CharNode *currNode = rootNode;

    for(char* x = (char*)strBeingSearched; *x; x++){
        char c = (char)tolower(*x);//derefrence
        int index = findIndex(c);
        //if that letter is not a valid character
        if(index == -1)
        {
            return false; //return false cause it cannot be in vocab

        }
        //else continue to the next letter of strBeingSearched
        else if(currNode->next[index]==nullptr){
            return false;
        }
        else {
            currNode = currNode->next[index];
        }
    }
    return true;
}

//helper to return index of the char
int tree::findIndex(char c){
    //an array containing the key of characters to their index
    char indexArr[31] = {'a', 'b', 'c', 'd' , 'e' , 'f' , 'g' , 'h' , 'i' , 'j' , 'k' , 'l' , 'm' , 'n' , 'o' ,
                               'p' , 'q' , 'r' , 's' , 't' , 'u' , 'v' , 'w' , 'x' , 'y' , 'z' , '\'' , '-', '_', ' ', '\0'};

    for(int i =0; i<31; i++){
        if(c == indexArr[i]){
            //returns index corresponding to the character
            return i;
        }
    }
    //returns -1 if the char is not a valid character
    return -1;
}