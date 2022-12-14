# SubnetCalculator
Two programs written in C++ to calculate subnets given a net IP address and some additional parameters (both FLSM and VLSM).

## Using FLSM
### What does it do
FLSM creates equal subnets starting from a single network. It takes the initial IP and mask and the number of subnets it needs to create.
### How to use it
flsm.exe [IP] [mask] [numberOfSubnets]
flsm.exe 128.64.16.0 20 6
### How does it work
The algorithm is very simple: first of all, it calculates how many bits of subnet mask will be allocated from the numberOfSubnets parameter. Then it just does some basic bitshift on the mask and prints all of the required subnets.

## Using VLSM
### What does it do
VLSM creates multiple subnets of different subnet mask to better distribute the addressing space.
This program always tries to allocate subnets from the **end** of the addressing space, to leave the 0 subnet always free for further subnetting.
### How to use it
vlsm.exe [IP] [mask] [hostPerSubnet...]
vlsm.exe 180.30.20.128 25 28 14 60
In the example above, three subnets are requested (one with 28 hosts, one with 14 hosts and another one with 60 hosts).
### How does it work
The algorithm this time is a little bit trickier than just a division and some bitwise operation.
The program recursively divides the subnet in two other subnets (with a subnet mask increased by one) until it reaches a perfect fit for the largest subnet it needs to generate. Then it repeates this process until all necessary subnets are created.
