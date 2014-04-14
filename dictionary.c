/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/
#include "dictionary.h"

// declare lobal variables 

unsigned int ctr = 0; 
node* Hashtable[HASHTABLE_SIZE] = {NULL};

/**
* Builds a new node and returns a pointer to a new node  
*/
node* buildnode(void)
{
    node* new_node = malloc(sizeof(node)); 
    if (new_node == NULL)
    {
        fprintf(stderr, "Out of memory"); 
        exit(1); 
    }
    return new_node;  
}
/*
 * Returns a hash value for a given word.
 */
unsigned int hash(const char* word)
{
    // source: http://www.cse.yorku.ca/~oz/hash.html
    
    unsigned int hash = 0;
    int c;
 
    while((c = *word++))
    {
       hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    hash = (hash % HASHTABLE_SIZE);
    if (hash > HASHTABLE_SIZE )
    {
        printf("Calculated hash value is too large.");
    }
    return hash;
}
/*
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // declare variables
    char WordtoCheck[LENGTH + 1];
    
    // convert upper case to lower case
    for (int i = 0; i < LENGTH + 1; i++)  
    {
        if (word[i] == '\'')
        {
            WordtoCheck[i] = '\'';
        }
        else 
        {
            WordtoCheck[i] = word[i];
            if ( isupper (WordtoCheck[i]) )
            {
                WordtoCheck[i] = tolower(WordtoCheck[i]);
            }
        }

    }    
    // iterate the crawler through the linked list until you find the word
    for (node* Crawler = Hashtable[hash(WordtoCheck)]; Crawler != NULL; Crawler = Crawler->next)
    {     
        int result = strcmp(Crawler->word, WordtoCheck); 
        if (result == 0)
        {
            return true;
        }   
    }
    return false;        
}
/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    //declaring variables 
    unsigned long Hash_Index = 0; 

    //open dictionary 
    FILE* dict = fopen(dictionary, "r");
    //check for successful open 
    if (dict == NULL)
    {
        printf("Could not open %s \n", dictionary);
        return false;
    }    
    //iterate over each word in the dictionary stopping after reaching the end of dictionary
    while (!feof(dict))
    {
        // reserve space for a new node and read from dictionary file
        node* new_node = buildnode(); 
        int result = fscanf(dict, "%s", new_node->word);
        if (result != 1)
        {
            break; 
        }
        else  
        //hash the word 
        Hash_Index = hash(new_node->word);
        
        //if it' the first word in the bucket then make it the first word in the bucket
        if (Hashtable[Hash_Index] == NULL)
        {
            Hashtable[Hash_Index] = new_node;
            new_node->next = NULL; 
            ctr++;
        }
        // prepend the node to the beginning of the list 
        else if (Hashtable[Hash_Index] != NULL)
        {
            new_node->next = Hashtable[Hash_Index];
            Hashtable[Hash_Index] = new_node;  
            ctr++;     
        }
    }
    fclose(dict);         
    return true; 
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (load)
    {
        return ctr;
    }
    else
    {
        return 0; 
    }
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    //free all the nodes
    
    //for each element in hashtable 
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        // for each element in linked in list 
        for (node* Crawler = Hashtable[i]; Crawler != NULL; Crawler = Crawler->next)
        {
            // free element move to next element 
            free(Crawler); 
        }
    }    
    
    return true;
}
