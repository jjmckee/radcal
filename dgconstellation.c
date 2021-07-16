#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAXBAND 20

struct metadata
{
    char * sensor;
    char * comb;
    bool pan;
    bool multi;
    bool ms1;
    bool ms2;
    bool swir;
    bool rgb;
    bool cir;
    int bands;
    int pbands;
    struct tm * datetime;
    float vangle;
    float resolution;
    float sunele;
    float theta;
    float abscal[MAXBAND];
    float bandwidth[MAXBAND];
    int index[MAXBAND];
    float gain[MAXBAND];
    float offset[MAXBAND];
    float irradiance[MAXBAND];
};

void setcomb(struct metadata * image)
{
    char * pcomb[7] = {"P",
                       "Multi",
                       "MS1",
                       "MS2",
                       "All-S",
                       "NRG",
                       "RGB"};

    image->pan = false;
    image->multi = false;
    image->ms1 = false;
    image->ms2 = false;
    image->swir = false;
    image->rgb = false;
    image->cir = false;

    int ind = -1;
    int result;

    for ( int i = 0; i < 7; i++ )
    {
        result = strcmp(pcomb[i], image->comb);
        if (result == 0)
        {
            ind = i;
            break;
        } 
        else
        {
            continue;
        }
    }  

    switch( ind )
    {
        case 0 :
            image->pan = true;
            break;
        case 1 :
            image->multi = true;
            break;
        case 2 :
            image->ms1 = true;
            break;
        case 3 :
            image->ms2 = true;
            break;
        case 4 :
            image->swir = true;
            break;
        case 5 :
            image->rgb = true;
            break;
        case 6 :
            image->cir = true;
            break;
        default:
            printf( "\nUnkown band combination (%s) encountered!\n\n", image->comb );
            exit(1);
    }   
}

void finalizestruct(struct metadata * image)
{   
    if ( strcmp(image->sensor, "QB02") == 0 )
    // BAND ORDER
    // PAN, BLUE, GREEN, RED, NIR
    {
        image->pbands = 5;
        float gain[5]  = {0.870, 
                          1.105, 
                          1.071, 
                          1.060, 
                          1.020};
        memcpy(image->gain, gain, sizeof gain);
        float offset[5] = {-1.491, 
                           -2.820, 
                           -3.338, 
                           -2.954, 
                           -4.722};
        memcpy(image->offset, offset, sizeof offset);
        float irradiance[5] = {1370.92,
                               1949.59, 
                               1823.63, 
                               1553.78, 
                               1102.85};
        memcpy(image->irradiance, irradiance, sizeof irradiance);
        if ( image->pan)
        {
            int index[1]  = {0};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->multi )
        {
            int index [4] = {1, 2, 3, 4};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->rgb )
        {
            int index[3] = {3, 2, 1};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->cir )
        {
            int index[3] = {4, 3, 2};
            memcpy(image->index, index, sizeof index);
        }
        else
        {
            printf( "\nUnknown spectral bands encountered for %s!\n\n", image->sensor );
            exit(1);
        }
    }

    else if ( strcmp(image->sensor, "WV01") == 0 )
    // BAND ORDER
    // PAN
    {
        image->pbands = 1;
        float gain[1]  = {1.016};
        memcpy(image->gain, gain, sizeof gain);
        float offset[1] = {-1.824};
        memcpy(image->offset, offset, sizeof offset);
        float irradiance[1] = {1478.62};
        memcpy(image->irradiance, irradiance, sizeof irradiance);

        if ( image->pan )
        {
            int index [1] = {0};
            memcpy(image->index, index, sizeof index);
        }
        else
        {
            printf( "\nUnknown spectral bands encountered for %s!\n\n", image->sensor );
            exit(1);
        }
    }

    else if ( strcmp(image->sensor, "GE01") == 0 )
    // BAND ORDER
    // PAN, BLUE, GREEN, RED, NIR
    {
        image->pbands = 5;
        float gain[5]  = {0.970,
                          1.053,
                          0.994,
                          0.998,
                          0.994};
        memcpy(image->gain, gain, sizeof gain);
        float offset[5] = {-1.926,
                           -4.437,
                           -4.175,
                           -3.754,
                           -3.870};
        memcpy(image->offset, offset, sizeof offset);
        float irradiance[5] = {1610.73,
                               1993.18,
                               1828.83,
                               1491.49,
                               1022.58};
        memcpy(image->irradiance, irradiance, sizeof irradiance);

        if ( image->pan )
        {
            int index [1] = {0};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->multi )
        {
            int index [4] = {1, 2, 3, 4};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->rgb )
        {
            int index[3] = {3, 2, 1};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->cir )
        {
            int index[3] = {4, 3, 2};
            memcpy(image->index, index, sizeof index);
        }
        else
        {
            printf( "\nUnknown spectral bands encountered for %s!\n\n", image->sensor );
            exit(1);
        }
    }

    else if ( strcmp(image->sensor, "WV02") == 0 )
    // BAND ORDER
    // PAN, COASTAL, BLUE, GREEN, YELLOW, RED, REDEDGE, NIR1, NIR2
    {
        image->pbands = 9;
        float gain[9]  = {0.942,
                          1.151,
                          0.988,
                          0.936,
                          0.949,
                          0.952,
                          0.974,
                          0.961,
                          1.002};
        memcpy(image->gain, gain, sizeof gain);
        float offset[9] = {-2.704,
                           -7.478,
                           -5.736,
                           -3.546,
                           -3.564,
                           -2.512,
                           -4.120,
                           -3.300,
                           -2.891};
        memcpy(image->offset, offset, sizeof offset);
        float irradiance[9] = {1571.36,
                               1773.81,
                               2007.27,
                               1829.62,
                               1701.85,
                               1538.85,
                               1346.09,
                               1053.21,
                               856.599};
        memcpy(image->irradiance, irradiance, sizeof irradiance);

        if ( image->pan )
        {
            int index [1] = {0};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->multi )
        {
            int index [8] = {1, 2, 3, 4, 5, 6, 7, 8};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->ms1 )
        {
            int index[4] = {2, 3, 5, 7};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->ms2 )
        {
            int index[4] = {1, 4, 6, 8};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->rgb )
        {
            int index[3] = {5, 3, 2};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->cir )
        {
            int index[3] = {7, 5, 3};
            memcpy(image->index, index, sizeof index);
        }
    }

    else if ( strcmp(image->sensor, "WV03") == 0 )
    // BAND ORDER
    // PAN, COASTAL, BLUE, GREEN, YELLOW, RED, REDEDGE, NIR1, NIR2, SWIR1-SWIR8
    {
        image->pbands = 17;
        float gain[17] = {0.950,
                          0.905,
                          0.940,
                          0.938,
                          0.962,
                          0.964,
                          1.000,
                          0.961,
                          0.978,
                          1.200,
                          1.227,
                          1.199,
                          1.196,
                          1.262,
                          1.314,
                          1.346,
                          1.376};
        memcpy(image->gain, gain, sizeof gain);
        float offset[17] = {-3.629,
                            -8.604,
                            -5.809,
                            -4.996,
                            -3.649,
                            -3.021,
                            -4.521,
                            -5.522,
                            -2.992,
                            -5.546,
                            -2.600,
                            -2.309,
                            -1.676,
                            -0.705,
                            -0.669,
                            -0.512,
                            -0.372};
        memcpy(image->offset, offset, sizeof offset);
        float irradiance[17] = {1575.41,
                                1757.89,
                                2004.61,
                                1830.18,
                                1712.07,
                                1535.33,
                                1348.08,
                                1055.94,
                                858.77,
                                479.019,
                                263.797,
                                225.283,
                                197.552,
                                90.4178,
                                85.0642,
                                76.9507,
                                68.0988};
        memcpy(image->irradiance, irradiance, sizeof irradiance);

        if ( image->pan )
        {
            int index[1] = {0};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->multi )
        {
            int index[8] = {1, 2, 3, 4, 5, 6, 7, 8};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->ms1 )
        {
            int index[4] = {2, 3, 5, 7};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->ms2 )
        {
            int index[4] = {1, 4, 6, 8}; 
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->rgb )
        {
            int index[3] = {5, 3, 2};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->cir )
        {
            int index[3] = {7, 5, 3};
            memcpy(image->index, index, sizeof index);
        }
        else if ( image->swir )
        {
            int index [8] = {9, 10, 11, 12, 13, 14, 15, 16};
            memcpy(image->index, index, sizeof index);
        }
        else
        {
            printf( "\nUnknown spectral bands encountered for %s!\n\n", image->sensor );
            exit(1);
        }
    }

    else
    {
        printf( "\nSensor %s has not yet been implemented!\n\n", image->sensor );
        exit(1);
    }
}

