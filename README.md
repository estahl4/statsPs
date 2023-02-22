# statsPs
simple C utility for descriptive statistics, percentiles and empirical hypothesis testing 


Usage: <stream of numbers> | statsPs [--help] [percentiles] [_ observed_values] [_ crit_val_alphas]
Input:
  reads a column of numbers from stdin (pipe '|' or '<infile')
    NB: don't just run statPs. It'll wait for input from the keyboard.
  percentiles is any number of args between 0 and 1
  a dash '_' argument separates percentiles from observed values
  observed_vals is any number of observed values
  a 2nd dash '_' argument separates observed values from critical value alphas
    (if no obs vals are given, two dashes are required '- -')
  crit_val_alphas is any number of 1-tailed alpha-levels
    (ranging from 0 to 1, i.e. 1-alpha for the upper tail)
    (randomization: after the critical value, a P(significance) if an obs val == the critical value is output)
Output (to stdout), <tab>-delimited:
  'x: <mean> sd: <sd> n: <n>[ <percentile arg>ile: <percentile>...][ P(x<<obs_val>): <P(x<obs_val)>...][ CV<alpha>: <critval> <Psig-if==critval>...]<newline>

// NB: statsPs expects input from stdin. You must `echo | statsPs` even to see the Usage message.
  
//     Compile with:
  
         gcc -o statsPs statsPs.c -lm 
  
You must have std C math library with LD_LIBRARY_PATH set. 
