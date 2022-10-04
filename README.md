# Compiler study project

## How to run

`mkdir build && cd build`

`cmake .../compiler`

`make`

## Instruction format

Instruction block contains instruction opcode and additional values that can be used as registers' numbers or immediate field
and two basic blocks pointers to use in jump and phi instructions.

## Basic block

Basic block contains list of instructions that can be added and two sets for predecessors and successors
Instuctions added manually while predecessors and successors based on graph edges

## Graph

Graph implemented as map where each block is assigned a set of successor blocks

## Testing

For testing added handlers for small set of isa, control flow checked with logging basic block execution
Data flow -- with counting factorial
