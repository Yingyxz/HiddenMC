
/*******************************************************************************

This program estimates a constant beta using OLS regression or, equivalently,
   MLE estimation.

*******************************************************************************/

// Global variables.
double *R, *M;

// Functions found below.
void GetData();
void Regress ();

#include "..\..\0Functions\Functions.h"

int main () {

   // Read in daily return data for the S&P 500 and JPM. Five years of data
   //    from 2012 through 2016.
   GetData();

   // Compute a constant beta via regression.
   Regress ();
   
   Exit ();

}

////////////////////////////////////////////////////////////////////////////////
void Regress () {

   int t;
   double num=0, den=0, beta, SOS=0, s_e;

   // Estimate beta.
   for (t = 1; t <= 1258; t++) {
      num += R[t] * M[t];
      den += M[t] * M[t];
   }
   beta = num / den;

   // Now calculate s_e.
   for (t = 1; t <= 1258; t++) {
      SOS += pow (R[t] - beta*M[t], 2);
   }
   s_e = sqrt(SOS / 1257.0);

   // Report.
   printf ("Beta = %8.3f, s_e = %8.3f\n", beta, s_e);

   return;

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



