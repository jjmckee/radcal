#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dgconstellation.c"
#include "date.c"

#define MAXCHAR 1000

// At-Sensor Radiance
// L = Gain * DN * ( abscalfactor / effectivebandwidth ) + Offset
// TOA Refelctance
// Rho = ( L * D^2 * pi ) / ( irradiance * cos(theta) )
// D = Earth-Sun distance ( TLCTime used to calculate D )
// theta = solar zenith angle or ( 90 - meanSunEl )

char * getkey(char * buff)
{
    char * key = malloc ( sizeof(char) * MAXCHAR );
    int c = 0;

    for ( int i = 0; i <= MAXCHAR; i++ )
    {
        if ( buff[i] == ' ' )
        {
            continue;
        }
        else if ( buff[i] == '"' )
        {
            continue;
        }
        else if ( buff[i] == '\t' )
        {
            continue;
        }
        else if ( buff[i] == '=')
        {
            key[c] = '\0';
            break;
        }
        else
        {
            key[c] = buff[i];
            c++;
        }
    }

    return key;
}

char * getitem(char * buff)
{
    char * item = malloc( sizeof(char) * MAXCHAR );
    char * subset;
    int c = 0;

    subset = strchr(buff, '=');

    for ( int i = 1; i <= MAXCHAR; i++ )
    {
        if ( subset[i] == ' ' )
        {
            continue;
        }
        else if ( subset[i] == '"' )
        {
            continue;
        }
        else if ( subset[i] == ';' )
        {
            continue;
        }
        else if ( subset[i] == '\n')
        {
            subset[i] = '\0';
            break;
        }
        else
        {
            item[c] = subset[i];
            c++;
        }
    }

    return item;
}

struct metadata * readmetadata(const char * arg)
{
    FILE * fp;
    char buff[MAXCHAR];
    struct metadata * pimage;
    pimage = (struct metadata *) malloc(sizeof(struct metadata));
    int result;
    int acount = 0;
    float abs[MAXBAND];
    int bcount = 0;
    float bw[MAXBAND];
    pimage->bands = 0;
    fp = fopen(arg, "r");

    if ( fp == NULL )
    {
        printf( "\nERROR: Unable to open file %s\n\n", arg );
    }
    else
    {
        while( fgets(buff, sizeof(buff), fp) != NULL )
        {
            if ( strstr(buff, "satId") != NULL )
            {
                pimage->sensor = getitem(buff);
            }
            else if ( strstr(buff, "bandId") != NULL )
            {
                pimage->comb = getitem(buff);
            }
            else if ( strstr(buff, "meanOffNadirViewAngle") != NULL )
            {
                pimage->vangle = atof(getitem(buff));
                setcomb( pimage );
            }
            else if ( strstr(buff, "meanCollectedRowGSD") != NULL )
            {
                pimage->resolution = atof(getitem(buff));
            }
            else if ( strstr(buff, "meanSunEl") != NULL )
            {
                pimage->sunele = atof(getitem(buff));
            }
            else if ( strstr(buff, "absCalFactor") != NULL )
            {
                abs[acount] = atof(getitem(buff));
                acount++;
                pimage->bands++;
            }
            else if ( strstr(buff, "effectiveBandWidth") != NULL )
            {
                bw[bcount] = atof(getitem(buff));
                bcount++;
            }
            else if ( strstr( buff, "firstLineTime") != NULL )
            {
                char * item = getitem(buff);
                pimage->datetime = getacq(item);
            }
            else
            {
                continue;
            }
        }
        fclose(fp);
    }
    memcpy(pimage->abscal, abs, sizeof abs);
    memcpy(pimage->bandwidth, bw, sizeof bw);
    pimage->theta = (90 - pimage->sunele);
    return pimage;
}

void getext(const char * arg)
{
    char p = '.';
    int ind;
    int size = strlen(arg);
    char * cext = ".IMD";
    char uext[10];
    int result;

    for ( int i = 0; i < size; i++ )
    {
        if ( arg[i] == p )
        {
            ind = i;
        }
        else
        {
           continue;
        }
    }

    for (int i = ind; i <= size; i ++)
    {
        uext[i - ind] = arg[i];
    }

    result = strcmp(uext, cext);

    if ( result != 0  )
    {
        printf( "\nERROR: %s is not a valid %s file!\n\n", arg, cext );
        exit(1);
    }
}

void checkargs(int argc, const char * argv[])
{
    if (argc > 2)
    {
        printf( "\nToo many arguments provided!\n\n" );
        exit(1);
    }
    else if (argc < 2 )
    {
        printf( "\nPlease provide a path to an image metadata (.IMD) file!\n\n" );
        exit(1);
    }
    else
    {
        getext(argv[1]);
        printf( "\nUser provided IMD: %s\n\n", argv[1] );
    }
}

int main(int argc, const char * argv[])
{
    struct metadata * image;
    checkargs(argc, argv);
    image = readmetadata(argv[1]);
    finalizestruct(image);

    // PRINTING FOR DEBUG PURPOSES

    printf( "Satellite Sensor: %s\n\n", image->sensor );
    printf( "Spatial Resolution: %f meters\n\n", image->resolution );
    printf( "View Angle: %f degrees\n\n", image->vangle );
    printf( "Sun Elevation: %f\n\n", image->sunele );
    printf( "Theta: %f\n\n", image->theta );

    if ( image->pan == true )
    {
        printf( "Band Combination: %s\\%s\n\n", image->comb, "Panchromatic" );
    }
    if ( image->multi )
    {
        printf( "Band Combination: %s\\%s\n\n", image->comb, "All Multispectral" );
    }
    if ( image->ms1 )
    {
        printf( "Band Combination: %s\\%s\n\n", image->comb, "Multispectral Subset 1" );
    }
    if ( image->ms2 )
    {
        printf( "Band Combination: %s\\%s\n\n", image->comb, "Mulitspectral Subset 2" );
    }
    if ( image->swir )
    {
        printf( "Band Combination: %s\\%s\n\n", image->comb, "Shortwave Infrared" );
    }
    if ( image->rgb )
    {
        printf( "Band Combination: %s\\%s\n\n", image->comb, "Natural Color" );
    }
    if ( image->cir )
    {
        printf( "Band Combination: %s\\%s\n\n", image->comb, "Color Infrared" );
    }
    printf( "Acquisition Date: %d/%d/%d\n\n", image->datetime->tm_mday, image->datetime->tm_mon, image->datetime->tm_year );
    printf( "Acquistion Time: %d:%d:%d\n\n", image->datetime->tm_hour, image->datetime->tm_min, image->datetime->tm_sec );
    printf( "Number of Bands: %d\n\n", image->bands );
    return 0;
}
