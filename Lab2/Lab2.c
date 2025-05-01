#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct word {
    int count;
    char *Word;
} WORD;

WORD *words[100000];
int totalw = 0;

int cmpword_alpha(const WORD *w1, const WORD *w2);
int cmpword_quant(const WORD *w1, const WORD *w2);

char *getword(FILE *F);
void add_word(char *word);

void write_output(FILE *out);
int is_text_file(const char *filename);

int main(int ac, char *av[]) {
	
    FILE *OUT1, *OUT2, *IN;
    char *pword;
    DIR *dir;
    struct dirent *ent;
    char path[1024];
    
    if (ac < 2) {
        printf("Dir 'texts' is not found\n"); 
        exit(0);
    }

    int i;
    for (i = 0; i < 100000; i++) {
        words[i] = NULL;
    }

    if ((dir = opendir(av[1])) == NULL) {
        printf("Couldn't open the dir %s\n", av[1]);
        exit(1);
    }

    while ((ent = readdir(dir)) != NULL) {

        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", av[1], ent->d_name);

        if (!is_text_file(path)) {
            continue;
        }

        IN = fopen(path, "r");
        if (IN == NULL) {
            printf("Couldn't open the file %s\n", path);
            continue;
        }

        printf("File processing %s\n", path);

        while ((pword = getword(IN)) != NULL) {
            add_word(pword);
            free(pword);
        }

        fclose(IN);
    }
    closedir(dir);
    
    OUT1 = fopen("alphabetical.txt", "w");
    OUT2 = fopen("frequency.txt", "w");
    if (OUT1 == NULL || OUT2 == NULL) {
        printf("Unable to create output files\n");
        exit(1);
    }
    
    qsort(words, totalw, sizeof(WORD *), 
          (int (*)(const void *, const void *))cmpword_alpha);
    write_output(OUT1);
    
    qsort(words, totalw, sizeof(WORD *), 
          (int (*)(const void *, const void *))cmpword_quant);
    write_output(OUT2);
    
    fclose(OUT1);
    fclose(OUT2);
    
    for (i = 0; i < totalw; i++) {
        free(words[i]->Word);
        free(words[i]);
    }
    
    return 0; 
}

void add_word(char *word) {

    int i;
    for (i = 0; i < totalw; i++) {
        if (strcmp(words[i]->Word, word) == 0) {
            words[i]->count++;
            return;
        }
    }
    
    if (totalw >= 100000) {
        printf("The maximum dictionary size has been exceeded\n");
        return;
    }
    
    WORD *new_word = (WORD *)malloc(sizeof(WORD));
    if (new_word == NULL) {
        printf("Memory segmentation error\n");
        exit(1);
    }
    
    new_word->Word = strdup(word);
    if (new_word->Word == NULL) {
        printf("Memory segmentation error\n");
        exit(1);
    }
    
    new_word->count = 1;
    words[totalw++] = new_word;
}

char *getword(FILE *F) {
    static char buffer[256];
    int c;
    int i = 0;
    
    while ((c = fgetc(F)) != EOF && !isalpha(c));
    
    if (c == EOF) return NULL;
    
    buffer[i++] = tolower(c);
    
    while (i < 255 && (c = fgetc(F)) != EOF && (isalnum(c))) {
        buffer[i++] = tolower(c);
    }
    
    buffer[i] = '\0';
    
    return strdup(buffer);
}

int cmpword_alpha(const WORD *w1, const WORD *w2)
	{
	const WORD *word1 = *(const WORD **) w1;
    const WORD *word2 = *(const WORD **) w2;
    
    return strcmp(word1->Word, word2->Word);

	}

int cmpword_quant(const WORD *w1,  const WORD *w2)
	{
	const WORD *word1 = *(const WORD **) w1;	
    const WORD *word2 = *(const WORD **) w2;
    
    if (word1->count > word2->count) return -1;
    if (word1->count < word2->count) return 1;
    
    return strcmp(word1->Word, word2->Word);
    
	printf("test_quant");
	}

void write_output(FILE *out) {
	int i;
    for (i = 0; i < totalw; i++) {
        fprintf(out, "%s: %d\n", words[i]->Word, words[i]->count);
    }
}

int is_text_file(const char *filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        return 0;
    }

    if (!S_ISREG(st.st_mode)) {
        return 0;
    }

    const char *dot = strrchr(filename, '.');
    if (dot && strcmp(dot, ".txt") == 0) {
        return 1;
    }
    return 1;
}
