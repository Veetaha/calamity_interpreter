#include <mem.h>
#include <error.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

// Copied following enum from dirent.h, as undefined reference are reported
/* File types for `d_type'.  */
enum
{
  DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
  DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
  DT_CHR = 2,
# define DT_CHR		DT_CHR
  DT_DIR = 4,
# define DT_DIR		DT_DIR
  DT_BLK = 6,
# define DT_BLK		DT_BLK
  DT_REG = 8,
# define DT_REG		DT_REG
  DT_LNK = 10,
# define DT_LNK		DT_LNK
  DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
  DT_WHT = 14
# define DT_WHT		DT_WHT
};



typedef struct dirent Dirent;

size_t Mem_readFile(const char * filePath, char * string, size_t stringSize) {
    if (filePath == NULL || string == NULL || stringSize < 1) return 0;
    FILE *file = fopen(filePath, "rb");
    if (file == NULL) return 0;
    //
    const size_t READ_BYTES = fread(string, sizeof(char), stringSize - 1, file);
    if (ferror(file)){
        fclose(file);
        return 0;
    }
    fclose(file);
    string[READ_BYTES] = '\0';
    return READ_BYTES;
}

bool Mem_fileExists(const char * filePath){
    FILE * file;
    if (filePath == NULL || (file = fopen(filePath, "rb")) == NULL){
        return false;
    }
    fclose(file);
    return true;
}


void * Mem_reallocate(void * ptr, size_t size){
    Debug_exists(ptr);
    //
    ptr = realloc(ptr, size);
    Error_checkHeap(ptr);
    return ptr;
}

void * Mem_malloc(size_t size){
    void * newbie = malloc(size);
    Error_checkHeap(newbie);
    return newbie;
}


void * Mem_calloc(size_t size){
    void * newbie = calloc(1, size);
    Error_checkHeap(newbie);
    return newbie;
}

size_t Mem_getFileSize(FILE * file){
    Debug_exists(file);
    //
    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    rewind(file);
    return size;
}

static signed char String_comp(String * left, String * right){
	return String_compare(left, right);
}

MemStatus Mem_getDirNames (const char   * directory, MemDirType type, Vector(String_ptr) * listOfNames){
    Debug_exists(directory);
    Debug_exists(listOfNames);
    //
    DIR * dir = opendir(directory);
    if (!dir) return MEM_FAILURE;
    Dirent * files = NULL;
    switch (type){
        case MEM_INDIR:{
            while ((files = readdir(dir))){
                if(files->d_type == DT_DIR && strcmp(files->d_name, "..") && strcmp(files->d_name, "."))
                    Vector_addToSorted(listOfNames, String_toNewString(files->d_name), &String_comp);
            }
            break;
        }
        case MEM_DIR:{
            while ((files = readdir(dir))){
                if(files->d_type == DT_DIR)
                    Vector_addToSorted(listOfNames, String_toNewString(files->d_name), &String_comp);
            }
            break;
        }
        case MEM_REG_FILE:{
            while ((files = readdir(dir))){
                if(files->d_type == DT_REG)
                    Vector_addToSorted(listOfNames, String_toNewString(files->d_name), &String_comp);
            }
            break;
        }
        //
        case MEM_DEFAULT:{
            while ((files = readdir(dir))){
                Vector_addToSorted(listOfNames, String_toNewString(files->d_name), &String_comp);
            }
            break;
        }
        case MEM_PUBLIC_DIR:{
            while ((files = readdir(dir))){
                if(files->d_type == DT_DIR && files->d_name[0] != '.')
                    Vector_addToSorted(listOfNames, String_toNewString(files->d_name), &String_comp);
            }
            break;
        }
        case MEM_PUBLIC_FILE:{
            while ((files = readdir(dir))){
                if(files->d_type == DT_REG && files->d_name[0] != '.')
                    Vector_addToSorted(listOfNames, String_toNewString(files->d_name), &String_comp);
            }
            break;
        }
        //
    }
    closedir(dir);
    return MEM_SUCCESS;

}



static bool nonSpace(char ch){
    return !isspace(ch);
}



void Mem_printFilesStatistic(const char * path,  size_t * letters, size_t * lines, size_t * words){

	auto_Vector(String_ptr) * files = Vector_new(String_ptr, String_free);
    Mem_getDirNames(path, MEM_REG_FILE, files);

    String * cur = String_new();
    auto_Vector(String_ptr) * wordsList = Vector_new(String_ptr, String_free);
    
    for (size_t i = 0; i < Vector_size(files); ++i){
        String_addCharFirst      (*Vector_at(files, i), '/');
        String_addCharsFirst     (*Vector_at(files, i), path);
        String_readFromPathString(*Vector_at(files, i), cur);

        String_appendNewWordsToVector(cur, (AnswerChar)nonSpace, wordsList);
        *words += Vector_size(wordsList);
        Vector_clear(wordsList);

        *letters += String_length(cur);
        size_t curLines = 1;
        for (size_t j = 0; j < String_length(cur); ++j){
            if (String_at(cur, j) == '\n')
                ++curLines;
        }
        *lines += curLines;
        printf("Lines: %4lu --> ", curLines);
        String_print(*Vector_at(files, i));
        putchar('\n');
    }

    String_free(cur);
}