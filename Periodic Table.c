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

    if((hashtable=malloc(sizeof(hashtable_t)))==NULL)
    {
        return NULL;
    }

    if((hashtable->table=malloc(sizeof(entry_t *) *size))==NULL)
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

int ht_hash(hashtable_t *hashtable, char *key)
{

    unsigned long int hashval;
    int i=0;

    while(hashval<ULONG_MAX && i<strlen(key))
    {
        hashval=hashval<<8;
        hashval+=key[i];
        i++;
    }

    return hashval % hashtable->size;
}

entry_t *ht_newpair(char *key, char *value)
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

void ht_set(hashtable_t *hashtable, char *key, char *value)
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
        }
        else
        {
            newpair->next=next;
            last->next=newpair;
        }
    }
}
char *ht_get(hashtable_t *hashtable, char *key)
{
    int bin=0;
    entry_t *pair;

    bin=ht_hash(hashtable, key);

    pair=hashtable->table[bin];
    while(pair!=NULL && pair->key!=NULL && strcmp(key,pair->key)>0)
    {
        pair=pair->next;
    }
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
    for(tok=strtok(line, " ");//delimiter
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

int main(char *argc,char *argv)
{	
	int choice=1;
	char line[1024];
    FILE *fptr;
    fptr=fopen("PeriodicTable.csv","r");
    if(fptr==NULL)
    {
        printf("Error opening file!! ");
        exit(1);
    }	
	
	char search[50]="";
    char prop[50]="";
    printf("Enter any information(from the following) of the element to be searched \n\n");
    printf("- Name(NM)\n");
    printf("- Formula(SS)\n");
    printf("- Atomic Number(AN)\n");
    printf("- Atomic Weight/Mass(AW)\n");
    printf("- Density(DE)\n");
    printf("- Melting Point(MP)\n");
    printf("- Boiling Point(BP)\n\n");
    
	printf("Enter any Entity: ");
    fgets(search,50,stdin);
    printf("Enter the Search Entity code: ");
    fgets(prop,50,stdin);
    
    strtok(search,"\n");

    while(fgets(line,1024,fptr))
    {
        if (line==NULL)
        {
            continue;
        }
        else
        {
            hashtable_t *table=ht_create(65536);
            char *tmp1=strdup(line);
            char *tmp2=strdup(line);
            char *tmp3=strdup(line);
            char *tmp4=strdup(line);
            char *tmp5=strdup(line);
            char *tmp6=strdup(line);
            char *tmp7=strdup(line);
            char name[50];
            char aw[50];
            char am[50];
            char de[50];
            char mp[50];
            char bp[50];
            char so[50];
            strcpy(name,getfield(tmp1,1));
            strcpy(aw,getfield(tmp2,2));
            strcpy(am,getfield(tmp3,3));
            strcpy(de,getfield(tmp4,4));
            strcpy(mp,getfield(tmp5,5));
            strcpy(bp,getfield(tmp6,6));
            strcpy(so,getfield(tmp7,7));
            ht_set(table,"NM",name);
            ht_set(table,"AN",aw);
            ht_set(table,"AW",am);
            ht_set(table,"DE",de);
            ht_set(table,"MP",mp);
            ht_set(table,"BP",bp);
            ht_set(table,"SS",so);
            if(strcmp(ht_get(table,trim(prop)),trim(search))==0)
            {
            	system("cls");
                printf("\n\n-----ELEMENT FOUND------");
				printf("\n\nName: %s\n",ht_get(table,"NM"));
                printf("Symbol: %s\n",ht_get(table,"SS"));
                printf("Atomic Number: %s\n",ht_get(table,"AN"));
                printf("Atomic Mass: %s\n",ht_get(table,"AW"));
                printf("Density: %s\n",ht_get(table,"DE"));
                printf("Melting Point: %s\n",ht_get(table,"MP"));
                printf("Boiling Point: %s\n",ht_get(table,"BP"));
            }
            else
            printf("Wrong Input!!");
        }
	}
}
