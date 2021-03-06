#--------------------------------#
# Example: 六边形区域上网格剖分(线条加密)　#
#--------------------------------#

#=========
| POINTS |
=========#
8 # number of points #

# Nodes which define the boundary #
0:  -0.5   -0.866   .005   1
1:  0.5   -0.866   .005   1
2:  1   0   .005   1
3:  0.5   0.866   .005   1
4:  -0.5  0.866  .005 1
5:  -1  0  .005  1

# Line for coarsing #
# Also we can use false hole #
7:  -0.99 0 0.2 0
6:  0.99 0 0.2 0

#===========
| SEGMENTS |
===========#
7 # Number of segments #

# Boundary segments #
0:   0   1   1
1:   1   2   1
2:   2   3   1
3:   3   4   1
4:   4   5   1
5:   5   0   1
6:   6   7   0
