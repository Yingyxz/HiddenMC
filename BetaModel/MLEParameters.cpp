
/*******************************************************************************

This program searches through an array of values for s_e (sigma_epsilon) and
   s_d (sigma_delta) to determine which values make the data most likely.

*******************************************************************************/

// Global variables.
double *R, *M;

// Functions found below.
void GetData();
void MLE ();

#include "..\..\0Functions\Functions.h"

int main () {

   // Read in daily return data for the S&P 500 and JPM. Five years of data
   //    from 2012 through 2016.
   GetData();

   // Compute the MLE estimates of sigma_epsilon and sigma_delta.
   MLE ();
   
   Pause ();

}

////////////////////////////////////////////////////////////////////////////////
void MLE () {

   int t;
   double r, m, mu, s_d, L, v, s_e, s2, sigma2,
          Lmax=-1e+50, s_e_max, s_d_max, twopi;

   twopi = 2.0 * 3.14159265358979;

   // Loop through values of s_e and s_d.
   for (s_e = 0.5; s_e <= 1.5; s_e += .001) {

      // Print this to show that it is thinking.
      printf (". ");

      // This quantity is used frequently below.
      s2 = s_e * s_e;

      for (s_d = 0.0; s_d <= 0.1; s_d += .001) {

         // The hyper-parameters.
         mu = 1.302;
         sigma2 = 0.034 * 0.034;

         // Initialize the log of the likelihood.
         L = 0.0;

         for (t = 1; t <= 1258; t++) {

            r = R[t];
            m = M[t];

            // Reflect the likelihood of this item of data (r).
            v = (sigma2 + s_d*s_d)*m*m + s2;
            L += (-log(sqrt(twopi*v)) - (r-m*mu)*(r-m*mu) / (2*v));

            // Now update mu and sigma2.
            mu = (s2*mu + (sigma2 + s_d*s_d)*r*m) / (s2 + m*m*(sigma2 + s_d*s_d));
            sigma2 = s2*(sigma2 + s_d*s_d) / (s2 + m*m*(sigma2 + s_d*s_d));

         }

         // See if the current (s_e, s_d) generates a new maximum likelihood.
         // If so, record the data.
         if (L >= Lmax) {
            Lmax = L;
            s_e_max = s_e;
            s_d_max = s_d;
         }
 
      }

   }

   // Report the results.
   printf ("\n");
   printf ("Best s_e is %8.5f, best s_d is %8.5f\n", s_e_max, s_d_max);

}

////////////////////////////////////////////////////////////////////////////////
// Allocate array space and read in S&P 500 and JPM daily returns for a 5 year
//    period.
////////////////////////////////////////////////////////////////////////////////
void GetData() {

   int t;
   char input[100];
   FILE *fp;

   // Array for JPM returns.
   R = (double *) calloc (1300, sizeof (double));

   // Array for S&P500 returns.
   M = (double *) calloc (1300, sizeof (double));

   fp = fopen ("JPMSPDaily.txt", "r");

   // Read in column headers
   fgets (input, 99, fp);

   // Now read in return data for 1258 days.
   for (t = 1; t <= 1258; t++) {

      fgets (input, 99, fp);
      sscanf (input, "%lf %lf", M+t, R+t);

   }

   fclose (fp);

   return;

}



