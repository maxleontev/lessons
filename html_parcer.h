#ifndef HTML_PARCER_H
#define HTML_PARCER_H

#include <unistd.h>
#include <string>

//-------------------------------------------------------------------
#define DEFAULT_FILES_DIR "/media/sf_mint/maxiemax"

extern std::string htmlFilesDir;
int HtmlParcer(char * buff, ssize_t size);

//-------------------------------------------------------------------

#endif // HTML_PARCER_H
