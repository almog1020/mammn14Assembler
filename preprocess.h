#ifndef PREPROCESS_H
#define PREPROCESS_H
#include "utils.h"
/* reading macros and laying them out in a new "file".am if the macro exists*/
resrvedNames * pre_process(Line **head, char *file_name);

#endif
