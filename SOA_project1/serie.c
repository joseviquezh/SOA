#include <gtk/gtk.h>
#include <string.h>

typedef struct _ConfigData {
  gint opMode; //0: Not expropriative 1:Expropriative
  gint threadsNum; //5 minimum
  gint ticketsNum; 
  gdouble workQuantity; //One unit is equivalent to 50 terms
  gint quantum; //In miliseconds
  gint workPercentage; //Between 0-100%
} ConfigData;

static void get_parameters( gpointer data )
{
  ConfigData *cdata = (ConfigData *)data;
  gint opMode = 0; //0: Not expropriative 1:Expropriative
  gint threadsNum = 5; //5 minimum
  gint ticketsNum = 10000; 
  gdouble workQuantity = 100; //One unit is equivalent to 50 terms
  gint quantum = 100; //In miliseconds
  gint workPercentage = 5; //Between 0-100%

  /* Read parameters file if exist or write a new one with default values */
  FILE *fin = fopen("parameters.cfg", "r");
  if (fin==NULL) {
    FILE *fout = fopen("parameters.cfg", "w");
    fprintf(fout,"operationMode=%d\n",opMode);
    fprintf(fout,"threadsNumber=%d\n",threadsNum);
    fprintf(fout,"ticketsNumber=%d\n",ticketsNum);
    fprintf(fout,"workQuantity=%lf\n",workQuantity);
    fprintf(fout,"quantum=%d\n",quantum);
    fprintf(fout,"workPercentage=%d\n",workPercentage);
    fclose(fout);
  } 
  else {
    fscanf(fin,"operationMode=%d\n",&opMode);
    fscanf(fin,"threadsNumber=%d\n",&threadsNum);
    fscanf(fin,"ticketsNumber=%d\n",&ticketsNum);
    fscanf(fin,"workQuantity=%lf\n",&workQuantity);
    fscanf(fin,"quantum=%d",&quantum);
    fscanf(fin,"workPercentage=%d",&workPercentage);
    fclose(fin);
  }
  cdata->opMode = opMode;
  cdata->threadsNum = threadsNum;
  cdata->ticketsNum = ticketsNum; 
  cdata->workQuantity = workQuantity;
  cdata->quantum = quantum;
  cdata->workPercentage = workPercentage;
}

static gdouble calculate_pi( gpointer data )
{
  ConfigData *cdata = (ConfigData *)data;
  gdouble i, terms = 50.0*cdata->workQuantity;
  gdouble num=1.0, den=2.0, prev=2.0, term=0.0;
  gdouble result = 2.0;

  for (i = 0;i<terms;i++){
    term = prev*(num/(den));
    prev = term;
    term = prev*(1/(den+1));
    num = num + 2.0;
    den = den + 2.0;
    result = result + term;
    g_print("terms %lf, i %lf, num %lf, den %lf, result %32.30lf\n", terms, i, num, den, result);
  }

  return result;
}

int main (int argc,
          char *argv[])
{
  ConfigData *cdata;

  cdata = g_malloc (sizeof (ConfigData));
  get_parameters( cdata );
  g_print ("%f",calculate_pi( cdata ));

  g_free (cdata);
  
  return 0;
}
