#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAXCHAR 1000

struct tm * getacq(char * datestr)
{
    struct tm * time;
    time = (struct tm *) malloc(sizeof(struct tm));
    char subset[5];
    int sc = 0;
    int wc = 0;

    for (int i = 0; i <= MAXCHAR; i++)
    {
        if ( datestr[i] == '.' )
        {
            subset[sc] = '\0';
            time->tm_sec = atoi(subset);
            sc = 0;
            break;
        }
        else if ( datestr[i] == '-' )
        {
            subset[sc] = '\0';
            if ( wc == 0 )
            {
                time->tm_year = atoi(subset);
                wc++;
            }
            else
            {
                time->tm_mon = atoi(subset);
                wc = 0;
            }
            sc = 0;
        }
        else if ( datestr[i] == 'T' )
        {
            subset[sc] = '\0';
            time->tm_mday = atoi(subset);
            sc = 0;
        }
        else if ( datestr[i] == ':' )
        {
            subset[sc] = '\0';
            if ( wc == 0 )
            {
                time->tm_hour = atoi(subset);
                wc++;
            }
            else
            {
                time->tm_min = atoi(subset);
            }
            sc = 0;
        }
        else
        {
            subset[sc] = datestr[i];
            sc++;
        }
    }
    return time;
}

//int main()
//{
//    char * datestr = "2014-11-09T05:55:53.430050Z";
//    struct tm * time; 
//
//    printf( "\nOriginal Date: %s\n\n", datestr );
//   
//    time = getacq(datestr);
//    
//    printf( "Date struct is: %d/%d/%d\n\n", time->tm_mday, time->tm_mon, time->tm_year );
//    printf( "Time struct is: %d:%d:%d\n\n", time->tm_hour, time->tm_min, time->tm_sec );
//}
