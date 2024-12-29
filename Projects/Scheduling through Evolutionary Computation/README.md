# Solving a Scheduling Task through Evolotionary Computing

### Overview

The premise of this project is a scheduling problem, of TAs to class sections. We are given preferences of the TAs, and requirements for the classes and we are attempting to construct schedules that fit these parameters best. 

### Methodology

This problem can be addressed through evolutionary computing. In short, this process will work by starting from a seed solution, then modifying it and adding the new solutions to a stored set of solutions. The next step is to evalate each solution in the set, and remove solutions which have not improved upon existing schedules. At each iteration of the algorithm, every solution is modified in some way and then the evaluatio and elimination process occurs. The idea behind this is the same as behind evolution, that if you continuously make changes to members, and then the "strongest" members survive, over thousands of years (or iterations) a population will become better on the basis of what you are evaluting it on. By creating objective numeric evaluators for each schedule, we were able to do this very well. By utilizing linearized numpy operations our code was able to come to a set of six optimal solutions in just a couple of minutes. 

