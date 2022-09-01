
/*******************************************************************************

This program uses OLS regression over 130 day periods to estimate beta_t.

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

   Pause ();

}

////////////////////////////////////////////////////////////////////////////////
void Regress () {

   int s, t;
   double num, den, beta, time[1300], bayes[1300];
   FILE *fp, *fp2;
   char input[100];


   fp = fopen ("Beta.txt", "r");
   for (t = 1; t <= 1258; t++) {
      fgets (input, 99, fp);
      sscanf (input, "%lf %lf", time+t, bayes+t);
   }
   fclose (fp);

   fp  = fopen ("Beta_130.txt", "w");
   fp2 = fopen ("Scatter.txt", "w");

   // Estimate beta.
   for (t = 131; t <= 1258; t++) {

      // Estimate beta over the period [t-130, t-1]
      num = den = 0.0;
      for (s = t-130; s < t; s++) {
         num += R[s] * M[s];
         den += M[s] * M[s];
      }
      beta = num / den;

      // Send to the output file.
      fprintf (fp, "%8.3f %8.3f\n", 60.0*t/1258.0, beta);

      // Show the two methodologies in a scatter plot.
      fprintf (fp2, "\\put {$\\cdot$} at %8.3f  %8.3f\n", bayes[t], beta);

   }

   fclose (fp);
   fclose (fp2);

   printf ("View the two time series for beta with BetaPlot.tex.\n");
   printf ("See a different presentation in ScatterPlot.tex.\n");
   Exit ();

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



