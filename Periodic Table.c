#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
struct entry_s
{
    char *key;
    char *value;
    struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s
{
    int size;
    struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;

hashtable_t *ht_create(int size)
{

    hashtable_t *hashtable=NULL;
    int i;

    if(size<1)
        return NULL;

    if((hashtable=malloc(sizeof(hashtable_t)))==NULL)                                                                                                       // Allocate the table itself.
    {
        return NULL;
    }

    if((hashtable->table=malloc(sizeof(entry_t *) * size))==NULL) // Allocate pointers to the head nodes.
    {
        return NULL;
    }
    for(i=0;i<size;i++)
    {
        hashtable->table[i]==NULL;
    }

    hashtable->size=size;

    return hashtable;
}

int ht_hash(hashtable_t *hashtable, char *key) // Hash a string for a particular hash table.
{

    unsigned long int hashval;
    int i = 0;

    while(hashval<ULONG_MAX && i<strlen(key)) // Convert our string to an integer
    {
        hashval=hashval<<8;
        hashval+=key[i];
        i++;
    }

    return hashval % hashtable->size;
}

entry_t *ht_newpair(char *key, char *value) // Create a key-value pair.
{
    entry_t *newpair;

    if((newpair=malloc(sizeof(entry_t)))==NULL)
    {
        return NULL;
    }

    if((newpair->key=strdup(key))==NULL)
    {
        return NULL;
    }

    if((newpair->value=strdup(value))==NULL)
    {
        return NULL;
    }

    newpair->next=NULL;

    return newpair;
}

void ht_set(hashtable_t *hashtable, char *key, char *value) // Insert a key-value pair into a hash table.
{
    int bin=0;
    entry_t *newpair=NULL;
    entry_t *next=NULL;
    entry_t *last=NULL;

    bin=ht_hash(hashtable, key);

    next=hashtable->table[bin];

    while (next!=NULL && next->key!=NULL && strcmp(key, next->key)>0)
    {
        last=next;
        next=next->next;
    }

    // There's already a pair.  Let's replace that string.
    if (next!=NULL && next->key!=NULL && strcmp(key, next->key)==0)
    {

        free(next->value);
        next->value=strdup(value);
    }
    else
    {
        newpair=ht_newpair(key, value);

        if (next==hashtable->table[bin])
        {
            newpair->next=next;
            hashtable->table[bin]=newpair;
        }
        else if (next==NULL)
        {
            last->next=newpair;

            // We're in the middle of the list.
        }
        else
        {
            newpair->next=next;
            last->next=newpair;
        }
    }
}

// Retrieve a key-value pair from a hash table.
char *ht_get(hashtable_t *hashtable, char *key)
{
    int bin=0;
    entry_t *pair;

    bin=ht_hash(hashtable, key);

    // Step through the bin, looking for our value.
    pair=hashtable->table[bin];
    while(pair!=NULL && pair->key!=NULL && strcmp(key, pair->key) >0)
    {
        pair=pair->next;
    }

    // Did we actually find anything?
    if (pair==NULL || pair->key==NULL || strcmp(key, pair->key)!=0)
    {
        return NULL;
    }
    else
    {
        return pair->value;
    }
}
const char *getfield(char *line, int num)
{
    const char *tok;
    for(tok=strtok(line, " ");
         tok && *tok;
         tok=strtok(NULL, " \n"))
    {
        if(!--num)
            return tok;
    }
    return NULL;
}
char *ltrim(char *s)
{
    while(isspace(*s))
        s++;
    return s;
}

char *rtrim(char *s)
{
    char *back=s+strlen(s);
    while(isspace(*--back))
	;
    *(back+1)='\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s));
}

int main(char *argc, char *argv)
{
    char line[1024];
    FILE *fptr;
    fptr=fopen("PeriodicTable.csv","r");
    if(fptr==NULL)
    {
        printf("Error opening file!! ");
        // Program exits if the file pointer returns NULL.
        exit(1);
    }
    
    printf("Enter any entitiy(from the following) of the element to be searched \n\n");
    printf("-Name\n");
    printf("-Formula\n");
    printf("-Atomic Number\n");
    printf("-Atomic Mass/Weight\n");
    printf("-Density\n");
    printf("-Melting Point\n");
    printf("-Boiling Point\n\n");
    printf(":");
    char search[50] = "";
    char prop[50] = "";
    fgets(search, 50, stdin);
    printf("Enter the Search entity: ");
    fgets(prop, 50, stdin);
    
    strtok(search, "\n");

    //fscanf(fptr, "%[^\n]", line);
    while (fgets(line, 1024, fptr))
    {
        //printf("%s \n", line);
        if (line == NULL)
        {
            continue;
        }
        else
        {
            hashtable_t *table = ht_create(65536);
            char *tmp1 = strdup(line);
            char *tmp2 = strdup(line);
            char *tmp3 = strdup(line);
            char *tmp4 = strdup(line);
            char *tmp5 = strdup(line);
            char *tmp6 = strdup(line);
            char *tmp7 = strdup(line);
            char name[50];
            char aw[50];
            char am[50];
            char de[50];
            char mp[50];
            char bp[50];
            char so[50];
            strcpy(name, getfield(tmp1, 1));
            strcpy(aw, getfield(tmp2, 2));
            strcpy(am, getfield(tmp3, 3));
            strcpy(de, getfield(tmp4, 4));
            strcpy(mp, getfield(tmp5, 5));
            strcpy(bp, getfield(tmp6, 6));
            strcpy(so, getfield(tmp7, 7));
            ht_set(table, "NM", name);
            ht_set(table, "AN", am);
            ht_set(table, "AW", aw);
            ht_set(table, "DE", de);
            ht_set(table, "MP", mp);
            ht_set(table, "BP", bp);
            ht_set(table, "SS", so);
            // /printf("%s %s \n", trim(prop), trim(search));
            if (strcmp(ht_get(table, trim(prop)), trim(search)) == 0)
            {
                printf("-----ELEMENT FOUND------");
				printf("\n\nName: %s\n", ht_get(table, "NM"));
                printf("Symbol: %s\n", ht_get(table, "SS"));
                printf("Atomic Number: %s\n", ht_get(table, "AN"));
                printf("Atomic Mass: %s\n", ht_get(table, "AW"));
                printf("Density: %s\n", ht_get(table, "DE"));
                printf("Melting Point: %s\n", ht_get(table, "MP"));
                printf("Boiling Point: %s\n", ht_get(table, "BP"));
            }
        }
        //printf("%s \n", getfield(line, 3))
    }

    //printf("Data from the file:\n%s", line);
    //fclose(fptr);

    // ht_set(hydrogen, "AN", "1");
    // ht_set(hydrogen, "AW", "2");
    // ht_set(hydrogen, "DE", "3");
    // ht_set(hydrogen, "MP", "4");

    // printf("%s\n", ht_get(hydrogen, "AN"));
    // printf("%s\n", ht_get(hydrogen, "AW"));
    // printf("%s\n", ht_get(hydrogen, "DE"));
    // printf("%s\n", ht_get(hydrogen, "MP"));
}
