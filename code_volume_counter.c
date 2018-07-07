#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <error.h>
#include <mem.h>
#include <list.h>
#include <sstring.h>
#include <stack.h>
#include <trie.h>
#include <hash_table.h>

int main(int argc, char ** argv){
    system("clear");
    setlocale(LC_CTYPE, "");

    size_t lines = 0;
    size_t letters = 0;
    size_t words = 0;


    Mem_printFilesStatistic("../include",
        &letters,
        &lines,
        &words);
    
    size_t lines2 = 0;
    size_t letters2 = 0;
    size_t words2 = 0;

    Mem_printFilesStatistic("../src",
        &letters2,
        &lines2,
        &words2);
    
    puts("\nFunny facts about my novel of dkr =)\n");


    printf("TOTAL LINES: %lu\nTOTAL LETTERS: %lu\nTOTAL WORDS: %lu\n", 
        lines + lines2, letters + letters2, words + words2);

    return 0;
}