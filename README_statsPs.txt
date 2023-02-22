
Apr 23, 2003
- changed 'CI' in the output and usage() to 'CV' for critical value
- changed '-' to '_' in usage()

Apr 3, 2003
- changed the character delimiting types of arguments to underscore '_', usage is:
[]$ <command> | statsPs <percentiles> [_ [<observed values>] _ <critical value alphas>]
Did it for Josh Akey at fhcrc.org, but more than a month too late. Sorry Josh!

October 19, 2002
- uses if(finite(vec[c])&&(vec[c]==vec[c])) instead of if(!isnan()) for portability (see 
http://molpopgen.org/krthornt/GCCSolaris.html)


October 18, 2002
- tarballed with -z, to make statsPs.tar.gz. 
- with
tar -xzf statsPs.tar.gz
will decompress into directory statsPs/, containing statsPs.c and README (this file)

Oct1 02 
- tarballed and placed on molpopgen.org 
- compile with 
[]$ gcc -o statsPs statsPs.c -lm
- then type
[]$ echo "" | statsPs --help

-> statsPs reads an empirical distribution from stdin, say, simulation results of a 
statistic. It expects a single column of numbers of any length. Run statsPs either as
[]$ <command> | statsPs <args>
or as
[]$ statsPs <args> <<input_file>
If you run statsPs without redirecting stdin, it will wait for input from the keyboard and 
appear frozen. 
-> statsPs accepts three kinds of arguments (output): percentile proportions (interpolated
percentiles), observed values (P-values, i.e.  P(x<obs_val)), and critical value alphas
(critical values (significant if obs_val more extreme than crit_val) followed by
randomization significance probabilities (if obs_val=crit_val)). Randomization deals with
discreteness in the distribution. (Note that the P-values output are not randomized; they are 
conservative.)
-> Help, argument formats and output formats are described by the command
[]$ statsPs --help
(remember to redirect stdin!)
-> This is a modification of Dick Hudson's stats.c. 




Sept 26, 2002 
- modified statsPs.c to read a third kind of arg, crit value alphas. (If no obs vals given, 
then include two '-'s on the command line, e.g. statsPs - - .025 .975.) Outputs (tab-
delimited): descriptor, critical_value (such that an observed value is significant if 
obs_val<crit_val), prob (randomization: the probability that an observed value is significant
if obs_val==crit_val).
- above modification was made to stats-randcrit.c, which was then copied to statsPs.c
- sym link created again. also done on eeg.

Sept 21, 2002
- modified statsPs.c to print 3 sig digits of %ile, e.g. "0.025ile	[value]"
- created a sym link '~/bin/statsPs' to '/home/elistahl/work/statsPs/statsPs' with the command
[elistahl@kingman statsPs]$ ln -s /home/elistahl/work/statsPs/statsPs  ~/bin/statsPs

- statsPs-mac.c is now portable (at least to kingman), so I did:
[elistahl@kingman statsPs]$ cp statsPs-mac.c statsPs.c
