/*  stats.c : Calculates mean, standard deviation for a list of numbers.
   Quantiles are also calculated if quantiles are specified by command 
line arguments. For example,  stats 0.05  0.5  0.95 <datafile
would output the mean, standard deviation (estimated from sample) and 
estimates of the  0.5, 0.5 and 0.95th quantile.  
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

main( int argc, char *argv[])
{
        char *inputstring;
	double *vec, x, s, *percentiles, *obsvals , *alphas, loP, hiP, P, min, max;
	int  c, vecl = 1000 ;
	int i, index,numpercentiles=0, numobsvals=0, numalphas=0, CVindex ;
	double p, sqrt();
	int order(int, double *) ;
	void usage(int);

	if ((argc>1)&&((argv[1][0]=='-')&&(argv[1][2]=='h'))) usage(0);
	inputstring = (char *)malloc( 100*sizeof(char) );
	if (argc>1) {
	  percentiles = (double *)malloc( (unsigned)argc*sizeof(double) );
	  obsvals = (double *)malloc( (unsigned)argc*sizeof(double) );
	  alphas = (double *)malloc( (unsigned)argc*sizeof(double) );
	}
	vec = (double *)malloc( (unsigned)vecl*sizeof( double) ) ;

	if (argc>1) {
	  for( i=1; i<argc; i++){
	    if (argv[i][0]=='_') { ++i; break; }
	    percentiles[i] = atof( argv[i] ) ;
	  }
	  numpercentiles=i-1;
	  if ((argc-i)>0)
	    numpercentiles--;
/* 	  printf("argc=%1d,numpercentiles=%1d\n",argc,numpercentiles); */
	  for(numobsvals=0 ; i<argc; i++) {
	    if (argv[i][0]=='_') { ++i; break; }
	    obsvals[numobsvals] = atof( argv[i] );
	    numobsvals++;
	  }
	  for(numalphas=0 ; i<argc; i++) {
	    alphas[numalphas] = atof( argv[i] );
	    numalphas++;
	  }
	}
/* 	printf("argc=%1d,i=%1d\n",argc,i); */
/* 	printf(" numpercentiles=%1d, numobs=%1d, numalphas=%1d\n",numpercentiles,numobsvals,numalphas); */
/* 	for (i=1;i<numpercentiles+1;i++) printf(" %5.3f",percentiles[i]); */
/* 	printf("\n"); */
/* 	for (i=0;i<numobsvals;i++) printf(" %5.3f",obsvals[i]); */
/* 	printf("\n"); */
/* 	for (i=0;i<numalphas;i++) printf(" %5.3f",alphas[i]); */
/* 	printf("\n"); */
/* 	exit (0); */
	order(numobsvals,obsvals);
	c = 0 ;
	x = s= 0.0 ;
/*  	fprintf(stderr,"GOT HERE\t\targ process \n"); */
/*  	    fprintf(stderr,"GOT IN HERE\n"); */
/*  	fprintf(stderr,"GOT HERE\t\tvec process \n"); */
/*  	fprintf(stderr,"GOT HERE\t\torder vec \n"); */

	while( scanf("%s",inputstring)==1 ) {
/* 	  printf("\nGOT IN HEREE"); */
	  vec[c] = atof(inputstring);
	  if (finite(vec[c])&&(vec[c]==vec[c])) {
/*  	    fprintf(stderr,"%1d,%lf\t",c,vec[c]); */
	    x += vec[c];
	    s += vec[c]*vec[c] ;
	    c++;
	    if( c >= vecl ) {
	      vecl += 1000;
	      vec = (double *)realloc( vec, (unsigned)vecl*sizeof(double) ) ;
	    }
	  }
	}
	if (c==0) usage(1);
/* 	} while( scanf("%s",inputstring)==1 ) ; */
/*  	fprintf(stderr,"GOT HERE\t\t sqrt \n"); */
/* 	if (c==0) { */
/* 	} */
/*  	for(i=0;i<c;i++) fprintf(stderr,"%6.4f\t",vec[i]); */
/*  	fprintf(stderr,"\n"); */
	order( c, vec);
	min=vec[0];
	max=vec[c-1];
	x /= c ;
	s /= c ;
	s -= x*x ;
	s = sqrt( (double)s*c/(c-1.0) ) ;
	printf("x:\t%lg\tse:\t%lg\tsd:\t%lg\tn:\t%d\trange:\t%lg\t%lg", x, s/sqrt(c), s, c,min,max);

	for( i=1; i<numpercentiles+1; i++) {
	  index = percentiles[i]*c + 0.5  ;
	  p = vec[index-1]*(index + 0.5 - percentiles[i]*c) 
	    + vec[index]*(percentiles[i]*c+0.5 -index) ;
	  printf("\t%.3gile:", percentiles[i]);
	  if( index < 1 ) printf("\t-");
	  else printf("\t%lg", p);
	}

	for (i=index=0;i<numobsvals;i++) {
	  printf("\tP(x<%.3g):",obsvals[i]);
	  for (;(index<c)&&(vec[index]<obsvals[i]);index++) ;
	  printf("\t%lg", ((double)index/c));
	}

	for (i=0;i<numalphas;i++) {
	  printf("\tCV%.4g:",alphas[i]);
	  CVindex=(int)(alphas[i]*c+0.00001) ;
	  if (alphas[i]<0.5) {
	    for (index=CVindex,loP=2.;fabs(vec[--index]-vec[CVindex])<DBL_EPSILON; )
	      if (index==0) { loP=0.; break; }
	    if (index==CVindex-1) P=0.;
	    else {
	      if (loP>1.) loP = ((double)index+1.)/c;
	      for (index=CVindex,hiP=2.;fabs(vec[++index]-vec[CVindex])<DBL_EPSILON; )
		if (index==c-1) { hiP=1.; break; }
	      if (hiP>1.5) hiP = (double)index /c;
	      P = (alphas[i]-loP)/(hiP-loP) ;
	    }
	  } else { //alphas[i]>0.5
	    CVindex--;
	    for (index=CVindex,loP=2.;fabs(vec[++index]-vec[CVindex])<DBL_EPSILON; )
	      if (index==c-1) { loP=0.; break; }
	    if (index==CVindex+1) P=0;
	    else {
	      if (loP>1.) loP = 1. - (double)index/c;
	      for (index=CVindex,hiP=2.;fabs(vec[--index]-vec[CVindex])<DBL_EPSILON; )
		if (index==0) { hiP=1.; break; }
	      if (hiP>1.5) hiP = 1. - ((double)index+1.) /c;
	      P = (1.-alphas[i]-loP)/(hiP-loP) ;
	    }
	  }
	  
	  printf("\t%lg\t%lg", vec[CVindex],P);
	}

	printf("\n");

}


       int
order(int n, double *pbuf)
{
        int gap, i, j;
        double temp;

        for( gap= n/2; gap>0; gap /= 2)
           for( i=gap; i<n; i++)
                for( j=i-gap; j>=0 && pbuf[j]>pbuf[j+gap]; j -=gap) {
                   temp = pbuf[j];
                   pbuf[j] = pbuf[j+gap];
                   pbuf[j+gap] = temp;
                   }
}


  void
usage(int exitstatus)
{
  fprintf(stderr,"\nUsage: statsPs [--help] [percentiles] [_ observed_values] [_ crit_val_alphas]\n");
  fprintf(stderr,"Input:\n");
  fprintf(stderr,"  reads a column of numbers from stdin (pipe '|' or '<infile')\n");
  fprintf(stderr,"    NB: don't just run statPs. It'll wait for input from the keyboard.\n");
  fprintf(stderr,"  percentiles is any number of args between 0 and 1\n");
  fprintf(stderr,"  a dash '_' argument separates percentiles from observed values\n");
  fprintf(stderr,"  observed_vals is any number of observed values\n");
  fprintf(stderr,"  a 2nd dash '_' argument separates observed values from critical value alphas\n");
  fprintf(stderr,"    (if no obs vals are given, two dashes are required '- -')\n");
  fprintf(stderr,"  crit_val_alphas is any number of 1-tailed alpha-levels\n");
  fprintf(stderr,"    (ranging from 0 to 1, i.e. 1-alpha for the upper tail)\n");
  fprintf(stderr,"    (randomization: after the critical value, a P(significance) if an obs val == the critical value is output)\n");
  fprintf(stderr,"Output (to stdout), <tab>-delimited:\n");
  fprintf(stderr,"  'x: <mean> sd: <sd> n: <n>[ <percentile arg>ile: <percentile>...][ P(x<<obs_val>): <P(x<obs_val)>...][ CV<alpha>: <critval> <Psig-if==critval>...]<newline>'\n\n");
  exit(exitstatus);
}
