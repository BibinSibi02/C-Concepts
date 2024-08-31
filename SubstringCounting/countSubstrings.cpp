//
// Created by Bibin Sibi
// Red Id: 825763777
// CS 480
#include "tree.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
int main(int argc, char **argv){
    std::ifstream infile(argv[1]);
    std::vector <std::string> vocab;
    std::string line;
    //create the vocab list in a vector
    while (std::getline(infile, line)){
        vocab.push_back(line);
    }
    infile.close();

    std::ifstream infile2(argv[2]);
    std::string line2;
    int ans = 0;
    while (std::getline(infile2, line2)){
        tree trie;
        const char* word = line2.c_str();
        //stores original length of the input string
        int stringLen = (int)strlen(word);
        for(int i =0; i < stringLen ;i++) {
            //add input to trie tree
            trie.insert(word);
            word++;
        }



        const char* vword;
        //for each word in the vocab vector, search if its a substring of input
            for(int i = 0; i <(int)vocab.size(); i++) {
                vword = vocab.at(i).c_str();
                if (trie.search(vword)) {
                    ans++;

                }
            }
            std::cout << ans << std::endl;
            ans = 0;

        }

    infile2.close();




}
