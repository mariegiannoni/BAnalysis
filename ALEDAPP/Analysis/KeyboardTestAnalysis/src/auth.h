#ifndef AUTH_H
#define AUTH_H


void createPassword(const char *filename, int length);


// This will need to be protected against running without admin rights:
char* readPassword(const char *filename);


// Password passed by address:
void cleanAndFreePassword(char **password);


#endif
