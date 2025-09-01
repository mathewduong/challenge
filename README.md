# acacia-challenge

This is the acacia-challenge project.

## The way I approached the problem

Reading the problem, I can tell immediately that it is a dynamic programming problem. There are usually specific keywords to a dynamic programming problem such as "Find X ways to form a sequence to Y". However, whenever I see a problem that asks for the number of ways to do something (especially with steps, positions, or constraints), I immediately consider dynamic programming, because it’s the most efficient way to count or optimize over a space of overlapping subproblems. This is especially true when I can see that the number of ways to reach a key at step i with v vowels is dependent on i-1 with v'. The problem depends on the previous subproblem, which is a key observation in dynamic programming.

Given that I know it is a dynamic programming problem, I now can approach it accordingly and aim to build an array to store solutions to the subproblems, which I can finally aggregate into a final total. In this way, we do not recompute what we do not have to.

## Building and installing

See the [BUILDING](BUILDING.md) document.
