

void readcsv(const char * arg)
{
    FILE * fp;
    char buff[MAXCHAR];

    if ( fp == NULL )
    {
        printf( "\nERROR: Unable to open file %s\n\n", arg );
    }
    else
    {
        while( fgets(buff, sizeof(buff), fp) != NULL )
        {
            if ( strstr(buff, "satId") != NULL )

