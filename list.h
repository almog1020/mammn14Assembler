#ifndef list_h
#define list_h
/*linked list to hold each line, each line ends with '\n' and max line length is 81*/
typedef struct Line{
    int line_i;
    int og_line;
    char line[81];
    struct Line *next;
}Line;
#endif
