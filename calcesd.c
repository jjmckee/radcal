#include <stdio.h>
#include <math.h>

int calcesd( int days )
{
    double e, pi, dtor, mtoau, theta, r;
    long a;

    e = 0.0167;
    //a = 149600000; //kilometers
    a = 149600000000; //meters
    pi = 3.14159265358979; 
    printf("\nPI: %f", pi);
    dtor = pi/180.0;
    mtoau = 1/149597870700.0;
    theta = (days * 360) / 365.25;
    printf( "\nTheta: %f\n\n", theta ); 
    r = a * ((1 - e * e)/(1 + e * cos(theta * dtor)));
    printf( "Earth Sun Distance (meters): %f\n\n", r ); 
    printf( "Earth Sun Distance (au): %f\n\n", r*mtoau ); 
}

int main()
{
    int days = 140;
    calcesd(days);
}
