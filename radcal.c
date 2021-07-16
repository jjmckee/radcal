#include <stdio.h>
#include <math.h>
#include "gdal.h"
#include "cpl_conv.h"

int main(int argc, const char * argv[])
{

    GDALDatasetH  ds;
    GDALRasterBandH band;
    GDALAllRegister();
    ds = GDALOpen( argv[1], GA_ReadOnly );

    if ( ds != NULL )
    {
        int rows = GDALGetRasterYSize( ds );
        int cols = GDALGetRasterXSize( ds );
        int bands = GDALGetRasterCount( ds );
        printf( "Size is %dx%dx%d\n", rows, cols, bands );
        int *pafScanline;
        pafScanline = (int *) CPLMalloc(sizeof(int)*cols); 
        int b, row, v;
        for ( b = 1; b <= bands; b++ )
        {
           band = GDALGetRasterBand( ds, b );
           printf("%d\n", GDALGetRasterDataType(band));
           for ( row = 0; row < rows; row++ )
           {
               GDALRasterIO(band, GF_Read, 0, row, cols, 1, pafScanline, cols, 1, GDT_UInt16, 0, 0);
               for ( v = 0; v < cols; v++ )
               {
                   int a = pafScanline[v] * sin(1.8);
               }
           }
        }
        CPLFree( pafScanline );
        GDALClose( ds ); 
    }
    else
        printf( "Unable to open dataset: %s!\n", argv[1] );
        exit(1);    
}
