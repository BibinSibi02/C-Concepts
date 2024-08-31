//
// Created by Bibin

#ifndef PA1SIBI_TREE_H
#define PA1SIBI_TREE_H
#define NCHILD 31
/* valid chars: a-z, ', -, _, single space, terminator of string */
class tree{
    typedef struct CharNode {
        struct CharNode *next[NCHILD];
        CharNode() {
            //initialize next array to array of nullptr
            for (int i = 0; i < 31; i++) {
                next[i] = nullptr;
            }
        }
    }CharNode;
private:
    CharNode *rootNode = new CharNode();

public:
    tree();

    bool insert(const char *stringBeingInserted);

    bool search(const char *strBeingSearched);

    //helper variable

    int findIndex(char c);
};

#endif //PA1SIBI_TREE_H
