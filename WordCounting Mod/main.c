#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h> 

#define WORD_LENGHT 64
#define MAPSIZE 1000000
#define THREADS 100

typedef struct hm_node 
{
	char key[WORD_LENGHT]; 
	int value; 
    struct hm_node* next;
} hm_node;



typedef struct scanned_file 
{

	char file_name[256]; 
	time_t mod_time; 
    int velicina;
   struct scanned_file* next;

} scanned_file;

struct scanned_file* Head = NULL;

pthread_mutex_t mutex[MAPSIZE];
pthread_t *threads[THREADS];

struct hm_node *hash_map[MAPSIZE];

char *stopwords[1000];
int stopwords_count = 0;
int thread_count = 0;

struct scanned_file* create_file(char* fileName){
    struct scanned_file* head = malloc(sizeof(struct scanned_file));
    strcpy(head->file_name, fileName);
    head->mod_time = NULL;
    head->velicina = NULL;
    head->next = NULL;
    return head;
}

struct scanned_file* DodajNaKraj(struct scanned_file* glava, char* fileName){
    struct scanned_file* novi=create_file(fileName);
    if(glava==NULL)
        return novi;

    struct scanned_file* tmp=glava;
    while(tmp->next!=NULL){
        tmp=tmp->next;
    }
    tmp->next=novi;
    return glava;
}
int daLiPostoji(struct scanned_file* glava, char* filename){
    struct scanned_file* tmp=glava;
    while(tmp!=NULL){
        if(strcmp(tmp->file_name,filename) == 0)
            return 1;
        tmp=tmp->next;
    }
    return 0;
}
struct scanned_file* getFile(struct scanned_file* glava, char* filename){
    struct scanned_file* tmp=glava;
    while(tmp!=NULL){
        if(strcmp(tmp->file_name,filename) == 0)
            return tmp;
        tmp=tmp->next;
    }
    return 0;
}
unsigned long djb2_hash(char* str) {
    unsigned long hash = 5381; // starting value
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash%MAPSIZE;
}

struct hm_node* create_node(char* rec, int broj) {
    struct hm_node* head = malloc(sizeof(struct hm_node));
    strcpy(head->key, rec);
    head->value = broj;
    head->next = NULL;
    return head;
}


int get_countRek(struct hm_node* node, char* rec){
    if(node == NULL)
        return -1;

    if(strcmp(node->key, rec) == 0)
        return node->value;

    return get_countRek(node->next, rec);
}
int get_word(char* rec) {
    int hash = djb2_hash(rec);
    pthread_mutex_lock(&(mutex[hash]));
    int count = get_countRek(hash_map[hash], rec);
    pthread_mutex_unlock(&(mutex[hash]));
    return count;
}
void init_rek(struct hm_node* curr,  char* rec, int hash) {
    if (curr == NULL) {
        pthread_mutex_init(&(mutex[hash]), NULL);
        struct hm_node* new_node = create_node(rec, 1);
        new_node->next = hash_map[hash];
        hash_map[hash] = new_node;
        pthread_mutex_unlock(&(mutex[hash]));
        return;
    }
    
    if (!strcmp(curr->key, rec)) {

        int tmp = curr->value + 1;
        curr->value = tmp;
        pthread_mutex_unlock(&(mutex[hash]));
        return;
    }
    
    init_rek(curr->next, rec, hash);
}

void increment_count(char* rec){
    int hash = djb2_hash(rec);
    struct hm_node* tmp = hash_map[hash];

    init_rek(tmp,rec,hash);

}
struct stat getStat(char *filePath) {
    struct stat st;

    stat(filePath, &st);
        
    return st;
}

void *scanner_work(void* argument){
    char *filePath = (char *)argument;
    struct scanned_file* scanned_file = getFile(Head,filePath);
    while (1)
    {
        struct stat st = getStat(filePath);

        
        if(scanned_file -> mod_time == st.st_mtime){
            sleep(5);
            continue;
        }

        
        FILE *file = fopen(filePath, "r");
        if (file == NULL)
        {
            printf("Error file is NULL %s\n", filePath);
            break;;
        } 

        char rec[64]; 
        int i = 0;
       for(int i = 0; i < scanned_file->velicina; i++){
            fscanf(file, "%c", rec);
        }
        while (fscanf(file, "%63s", rec) != EOF)
        {
            // 
            int valid = 1;
            for(int i = 0;i<strlen(rec);i++){
                if(!isalpha(rec[i])){
                    valid = 0;
                    break;
                }
                rec[i] = tolower(rec[i]);
            }
            int flag = 1;
             for(int i = 0; i < stopwords_count; i++){
                 if(!strcmp(rec, stopwords[i])){
                    flag = 0;
                    break;
                    }

    }
            if(valid && flag)
                increment_count(rec);
        }

        
        fclose(file);

        
        scanned_file->mod_time = st.st_mtime;
        scanned_file->velicina = st.st_size;

        
        sleep(5);
    }
}




int add_stopwords(char* filename){
        
        FILE *f = fopen(filename, "r");
        if (f == NULL)
        {
            printf("Fille is null, cant read\n");
            return 0;
        } 

        char buffer[64]; 
        while (fscanf(f, "%s", buffer) != EOF)
        {
            stopwords[stopwords_count] = (char*) malloc(sizeof(char)*(strlen(buffer)+1));
            
            strcpy(stopwords[stopwords_count++], buffer);
        }
        fclose(f);
        return 1;
}

void free_all(struct scanned_file* glava){
    for(int i = 0; i<stopwords_count; i++){
        free(stopwords[i]);
    }
 
    for(int i = 0; i<THREADS; i++){
        free(threads[i]); 

    while(glava!=NULL){
        struct scanned_file* tmp=glava;
        glava=glava->next;
        free(tmp);
    }
}
}
int main(int argc, char *argv[]){

    if(argc==2 && add_stopwords(argv[1]) != 0)
        return 0;
    
    char buffer[100];
    

    while (1)
    {
        printf("Command: ");
        fgets(buffer, 100, stdin);
        buffer[strlen(buffer)-1] = 0;

        if(strcmp(buffer, "_stop_") == 0) {
            break;
        }

        char provera[7] = {"_count_"};
        int flag = 0;
        for(int i = 0; i<7; i++){
            if(provera[i] != buffer[i])
                flag = 1;
                break;
            }
        
        if(!flag) {
            
            char filename[100];
            strcpy(filename, buffer+8);
                
            if(daLiPostoji(Head,filename) == 1){
                printf("Fajl skeniran\n");
                continue;
            }
                
            Head = DodajNaKraj(Head,filename);

            threads[thread_count] = malloc(sizeof(pthread_t));
            pthread_create(threads[thread_count++], NULL, scanner_work,filename);
                         
        }
        else{
                
                int end;
                char* rec;

                end = strlen(buffer);
                while (end >= 0) {
                    if (buffer[end] == ' ')
                        break;
                    end--;
                }

                
                if (end) 
                    rec = buffer + end + 1;
                 else 
                    rec = buffer;

                printf("Frekvencija %s: %d\n", rec, get_word(rec));
        }
        provera[0] = '\0';
    }

    free_all(Head);
    return 0;
}