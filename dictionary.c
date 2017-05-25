/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"

#define NUM_NODES 27
#define APOSTROPHE 26

// definte node struct
typedef struct node
{
    bool is_word;
    struct node *children[NUM_NODES];
}
trie_node;

// initialize root of trie
trie_node root = { .is_word = false };

// initialize counter variable
int num_words = 0;

// converts ascii value to trie node's children array index value
int character_converter(char letter)
{
    if (letter == '\'')
    {
        return APOSTROPHE;
    }
    else
    {
        return letter - 'a';
    }
}

bool trie_add(trie_node *node, const char *word)
{
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        // convert ascii value to array index value
        int node_number = character_converter(word[i]);
        
        // remember parent node
        trie_node *parent = node;
        
        // if child node not already created, calloc space for it
        if (node->children[node_number] == false)
        {
            // use calloc to initialize memory to 0
            trie_node *child = calloc(1, sizeof(trie_node));
            if (child == NULL)
            {
                return false;
            }
            // set pointer in parent's children array to point to new node
            parent->children[node_number] = child;
            // update current node to child
            node = child;
        }
        else // child node already exists
        {
        // go to child ndoe
        node = node->children[node_number];
        }
    }
    // set flag to true
    node->is_word = true;
    // increment dictionary size counter
    num_words++;
    return true;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // remember current node and set it to the root of the trie
    trie_node *current = &root;
    
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        // ignore capitalization
        char letter = tolower(word[i]);
        // convert ascii value to array index value
        int node_number = character_converter(letter);
        
        // if appropriate child node does not exist, word is misspelled
        if (current->children[node_number] == false)
        {
            return false;
        }
        // update current node to child
        current = current->children[node_number];
    }
    // return flag of last child node
    return current->is_word;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *file = fopen(dictionary, "r");
    // return false if dictionary file could not be opened
    if (file == false)
    {
        return false;
    }
    
    char word[LENGTH];
    
    // read in words from dictionary
    while (fscanf(file, "%s", word) != EOF)
    {
        // add words to trie
        if (trie_add(&root, word) == false)
        {
            // close dictionary and return false if loading fails
            fclose(file);
            return false;
        }
    }
    
    // close file pointer once dictionary is loaded
    fclose(file);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return num_words;
}

void delete_node(trie_node *current)
{
    // iterate over current node's children
    for (int i = 0; i < NUM_NODES; i++)
    {
        // if a child node exists, delete it
        if (current->children[i])
        {
            delete_node(current->children[i]);
        }
    }
    
    // root node globally defined, not dynamically allocated. 
    // do not free
    if (current == &root)
    {
        return;
    }
    
    // free current node
    free(current);
    current = NULL;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    delete_node(&root);
    return true;
}
