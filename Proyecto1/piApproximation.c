#include <stdio.h>
#include <math.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////
///////////////REFERENCES////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///http://numbers.computation.free.fr/Constants/Algorithms/nthdecimaldigit.pdf
///////////////////////////////////////////////////////////////////////////
///https://bellard.org/pi/pi2700e9/pipcrecord.pdf
///////////////////////////////////////////////////////////////////////////



#define MIN_OF_WORK 50

struct piProgress{
    double result;
    double divisor;
    double sign;
    double piSoFar;
    long long iterations;
};
typedef struct piProgress LookUp;

///////////////////////////////////////////////////////////////////////////
///////////////Gregory////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

double pi_gregory(long long int n){
    /*
    Calculate n iterations of Gregory's series
    https://www.craig-wood.com/nick/articles/pi-gregorys-series/
    */
    double result = 0;
    double divisor = 1;
    double sign = 1;
    int i=0;
    for (i=0;i<n;i++){
        result += sign / divisor;
        divisor += 2;
        sign = -1*sign;
    }
    return 4 * result;
}



void pi_gregory_pauseable(long long int n,LookUp* ptrProgress){
    /*
    Calculate n iterations of Gregory's series
    https://www.craig-wood.com/nick/articles/pi-gregorys-series/
    */
    int i=0;
    for (i=0;i<n;i++){
        ptrProgress->result += ptrProgress->sign / ptrProgress->divisor;
        ptrProgress->divisor += 2;
        ptrProgress->sign = -1*ptrProgress->sign;
        ptrProgress->iterations++;
    }
    ptrProgress->piSoFar=4 * ptrProgress->result;
}


LookUp* getInitState(){
   LookUp* ptrPiAproximation=(LookUp*)malloc(sizeof(LookUp));
   ptrPiAproximation->piSoFar=0;
    ptrPiAproximation->result = 0;
    ptrPiAproximation->divisor = 1;
    ptrPiAproximation->sign = 1;
    ptrPiAproximation->iterations=0;
    return ptrPiAproximation;
}


void testPi_gregory(){
    /*Try Gregory's series*/
    long long int fractionValue,fractionSpeed,log_n=0,n=0;double result;
    LookUp* ptrPiAproximationExpro=getInitState();
    LookUp* ptrPiAproximationNOExpro=getInitState();

    printf("Simple Call\n");
    for (log_n=0;log_n<=8;log_n++){
        n = pow(10,log_n);
        result=pi_gregory(n);
        printf(" %.64lf   with %lld iterations\n" , result, n);
    }

    printf("\n\n\nCall funtion in expropiative \n");
    printf("Calculate iterations of Gregory's series\n");
    printf("CORRE SEGUIDO EL SCHEDULER DEBE VER CUANDO LE QUITA EL PROCESADOR-----------\n");
    for (log_n=0;log_n<=8;log_n++){
        n = pow(10,log_n);
        pi_gregory_pauseable(n,ptrPiAproximationExpro);
        printf(" %.64lf   with %lld iterations\n" , ptrPiAproximationExpro->piSoFar, n);
    }



    printf("\n\n\nCall funtion in NO expropiative mode every 10 perc \n");
    printf("Calculate iterations of Gregory's series\n");
    for (log_n=0;log_n<=8;log_n++){
        n = pow(10,log_n);
        result = pi_gregory(n);
        fractionSpeed=10; //10 %
        fractionValue=(long int)(n/fractionSpeed);
        while(fractionSpeed-->0){
            pi_gregory_pauseable(fractionValue,ptrPiAproximationNOExpro);
            printf("At %lld percent it looks like %.64lf \n",10*(10-fractionSpeed),ptrPiAproximationNOExpro->piSoFar);

            printf("AQUI EL HILO DEBE SOLTAR EL PROCESADOR------------------");

        }
        printf("FINAL %.64lf  with %lld iterations\n---------------------\n" , ptrPiAproximationNOExpro->piSoFar, n);
    }

}


///////////////////////////////////////////////////////////////////////////
////////////////Archimedes///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


double pi_archimedes(long long int n){
    /*
    Calculate n iterations of Archimedes PI recurrence relation
    https://www.craig-wood.com/nick/articles/pi-archimedes/
    */
    double polygon_edge_length_squared,polygon_sides;
    long int i=0;
    polygon_edge_length_squared = 2.0;
    polygon_sides = 2.0;
    for(i=0;i<n;i++){
        polygon_edge_length_squared =2 - 2 * sqrt((1 - polygon_edge_length_squared / 4));
        polygon_sides *= 2;
    }
    return polygon_sides * sqrt(polygon_edge_length_squared);
}

void testPi_archimedes(){
    /*Archimedes Relation*/
    long long int log_n=0,n=0;
    double result;
    printf("Calculate n iterations of Archimedes PI recurrence relation\n");
    for (log_n=0;log_n<=25;log_n+=2){
        n = log_n;
        result = pi_archimedes(n);
        printf(" %.64lf   with %lld sides\n" , result, (long long int)(4*pow(2,n)));
    }

}

///////////////////////////////////////////////////////////////////////////
////////////////Chudnovsky///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


double newton_sqrt(int n, int one){
    /* Return the square root of n as a fixed point number with the one
     passed in.  It uses a second order Newton-Raphson convergence.  This
     doubles the number of significant figures on each iteration.
     ---- Use floating point arithmetic to make an initial guess
     */
    double double_point_precision = pow(10,16);
     double n_double = ((double)(n * double_point_precision) /one) / double_point_precision;
     double x_old,x = ((long int)((double_point_precision * sqrt(n_double)) * one))/ double_point_precision;
     double n_one = n * one;
     while(1){
         x_old = x;
         x = (x + n_one / x) / 2;
         if (x == x_old)
             break;
     }
     return x;
 }


 double pi_chudnovsky(long long int one){
    /*
    Calculate pi using Chudnovsky's series
    This calculates it in fixed point, using the value for one passed in
    */
   double pi;
    long int total,k = 1;
    long int a_k = one;
    long int a_sum = one;
    long int b_sum = 0;
    long int C = 640320;
    long int C3_OVER_24 =(long int) pow(C,3)/ 24;
    while (1){
        a_k *= -1*((6*k-5)*(2*k-1)*(6*k-1));
        a_k /= k*k*k*C3_OVER_24;
        a_sum += a_k;
        b_sum += k * a_k;
        k += 1;
        if(a_k == 0)
            break;
    }
    total = 13591409*a_sum + 545140134*b_sum;
    pi = (426880*newton_sqrt(10005*one, one)*one)/total;
    return pi;
 }

void testPi_chudnovsky(){
    /*Try Chudnovsky's series*/
    long long int log_n=0,n=0;
    double result;
    printf("Calculate iterations of chudnovsky's series\n");
    for (log_n=0;log_n<=8;log_n++){
        n =log_n+1;
        result = pi_chudnovsky(n);
        printf(" %.64lf with %lld iterations\n" , result, n);
    }

}


double piCaller(int workQuantity){
    return pi_gregory(workQuantity*MIN_OF_WORK);
}


/*int main(){
    testPi_gregory();
    //testPi_archimedes();
   // testPi_chudnovsky();
}*/
