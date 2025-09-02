# Acacia Challenge

This is the acacia-challenge project.

## The way I approached the problem

Reading the problem, I can tell immediately that it is a dynamic programming problem. There are usually specific keywords to a dynamic programming problem such as "Find X ways to form a sequence to Y". However, whenever I see a problem that asks for the number of ways to do something (especially with steps, positions, or constraints), I immediately consider dynamic programming, because it’s the most efficient way to count or optimize over a space of overlapping subproblems. This is especially true when I can see that the number of ways to reach a letter at step `i` with `v` vowels is dependent on `i-1` with `v'`. The problem depends on the previous subproblem, which is a key observation in dynamic programming.

Given that I know it is a dynamic programming problem, I now can approach it accordingly and aim to build an array to store solutions to the subproblems, which I can finally aggregate into a final total. In this way, we do not recompute what we do not have to. I first create test cases in tests in `test/source` to test my implmentation of `get_knight_moves`. Once the knight moves logic was validated, I proceeded to build the dynamic programming solution, using a 3-D array I store the solution of each step in the form of `dp_array[letter][seq][vowel]` then I aggregate the total.

### Visualisation for DP array

| Letter | 0th-seq      | ... | (sequence_length-1)th-seq |
| ------ | ------------ | --- | ------------------------- |
| "A"    | [v0, v1, v2] | ... | [v0, v1, v2]              |
| "B"    | [v0, v1, v2] | ... | [v0, v1, v2]              |
| ...    | ...          | ... | ...                       |

Where `[v0, v1, v2]` represents the count of sequences for each possible number of vowels used so far, with the index `vi` corresponding to `i` vowels used:

| Index | Symbol | Vowel Count | Description |
| ----- | ------ | ----------- | ----------- |
| 0     | v0     | 0           | No vowels   |
| 1     | v1     | 1           | 1 vowel     |
| 2     | v2     | 2           | 2 vowels    |

## Building and installing

See the [BUILDING](BUILDING.md) document.
