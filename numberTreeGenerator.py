#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Jul 18 18:45:15 2021

@author: kabirsheth
"""
import random as rand
from typing import List, Union 
from dataclasses import dataclass
from inspect import signature
from coyote.coyote_ast import CompilerV2, Var, Tree, Op
from coyote.vectorize_circuit import vectorize
from sys import argv
from collections import deque
import os
from coyote import *
 
# Expression = Union['Var', 'Op']

def treeGenerator(originalDepth, maxDepth, inputSeed, regime: str):
    global seed 
    seed = inputSeed
    rand.seed(seed)
    localString = ""
    if (originalDepth == maxDepth or maxDepth == originalDepth - 1):
        randomNum = (rand.randrange(0, 2) if regime == '100-100' else rand.randrange(0, 2))
        lhs = treeGenerator(originalDepth, maxDepth-1, seed, regime)
        rhs = treeGenerator(originalDepth, maxDepth-1, seed, regime)
        if (randomNum == 1):
            return(lhs + rhs)
        elif (randomNum == 0):
            return(lhs * rhs)
    if (maxDepth > 0):
        randomNum = rand.randrange(0, 4) if regime == '50-50' else (rand.randrange(0, 2) if regime == '100-50' else rand.randrange(0, 1))
        seed+=1
        if (randomNum > 1):
            localString+=str(rand.randrange(0,1024))
            seed+=1
            print(originalDepth + 1 - maxDepth)
            return Tree(Var(localString))
        else:
            lhs = treeGenerator(originalDepth, maxDepth-1, seed, regime)
            rhs = treeGenerator(originalDepth, maxDepth-1, seed, regime)
            if (randomNum == 1):
                return(lhs + rhs)
            elif (randomNum == 0):
                return(lhs * rhs)
    else:
        endNode = str(rand.randrange(0,1024))
        seed+=1
        return Tree(Var(endNode))

##################################################################

def build_polynomial_tree_from_file(tokens: deque):
    while tokens:
        if tokens[0] == "(":
            tokens.popleft()  # Remove '('
            operation_string = tokens.popleft()  # Extract operation
            potential_step = ""
            
            if tokens[0] == "(":
                operand1 = build_polynomial_tree_from_file(tokens)
            else:
                var_name = tokens.popleft()
                operand1 = Tree(Var(var_name))


            if tokens[0] == "(":
                operand2 = build_polynomial_tree_from_file(tokens)
                potential_step += " "
            elif tokens[0] != ")":
                var_name = tokens.popleft()
                operand2 = Tree(Var(var_name))
                potential_step = var_name

            if tokens[0] == ")":
                tokens.popleft()  # Remove ')'

            if potential_step:
                if operation_string == "+":
                    return (operand1 + operand2)
                elif operation_string == "*":
                    return (operand1 * operand2)

        else:
            var_name = tokens.popleft()
            return Tree(Var(var_name))

    raise ValueError("Invalid expression")

######################################################################################
######################################################################################
######################################################################################

class coyote_compiler:
    def __init__(self):
        self.func_signatures = {}
        self.outputs = []

    def vectorize(self):
        return vectorize(self.compiler)


    def instantiate(self,load_from_file,depth, seed, regime):
        outputs = []
        if load_from_file:
            def process_from_file(depth, regime):
                filename = f"polynomial_trees/tree_{regime}_{depth}.txt"
                try:
                    with open(filename, "r") as file:
                        polynomial_arithmetic_circuit = file.readline().strip()
                        tokens = deque(polynomial_arithmetic_circuit.split())
                        return build_polynomial_tree_from_file(tokens)
                except FileNotFoundError:
                    print(f"File {filename} not found")
                    return None

            # Handling cases
            cases = {
                ('5', '50-50'): lambda: print("Processing depth=5 and regime=50-50"),
                ('5', '100-50'): lambda: print("Processing depth=5 and regime=100-50"),
                ('5', '100-100'): lambda: print("Processing depth=5 and regime=100-100"),
                ('10', '50-50'): lambda: print("Processing depth=10 and regime=50-50"),
                ('10', '100-50'): lambda: print("Processing depth=10 and regime=100-50"),
                ('10', '100-100'): lambda: print("Processing depth=10 and regime=100-100"),
            }
            # If found in cases, execute corresponding action; otherwise, load from file
            output = cases.get((depth, regime), lambda: process_from_file(depth, regime))()
        else:
            output = treeGenerator(depth, depth, seed, regime)

        if output is not None:
            outputs.append(output)

        self.compiler = CompilerV2([])
        self.outputs = []
        
        for out in outputs:
            print(out.a)
            self.outputs.append(self.compiler.compile(out.a).val)

        return [' '.join(f'%{reg}' for reg in self.outputs)] + list(map(str, self.compiler.code))

def usage():
    print(f'Usage: {argv[0]} [list|build] [benchmark_name?]')
    raise SystemExit()
##########################################################################
################################################################################
if __name__ == '__main__':
    coyote = coyote_compiler()
    if (int(argv[2][-3]) == 5):
        depth = 5
    else:
        depth = 10
    seed = 9100 + (int(argv[2][-1]) - 1) * 100 + (int(argv[2][-3]) * 100)
    print(depth)
    print(seed)
    if len(argv) < 2:
        usage()
    command = argv[1]
    #############################################
    ######################################################################
    if command == 'list':
        print('List of available benchmarks:')
        for func in coyote.func_signatures:
            print(f'* {func.__name__}')
    elif command == 'build':
        if len(argv) != 3:
            print(f'Error: command "build" but no benchmark was specified (try `{argv[0]} list` to list available benchmarks)')
            usage()
        benchmark_name = argv[2]
        regime_index = benchmark_name[5:6]
        cases = {
            '0' : '50-50',
            '1' : '100-50',
            '2' : '100-100'
        }
        regime = cases[regime_index]
        print(regime)
        load_from_file = True
        scalar_code = coyote.instantiate(load_from_file,depth, seed + int(argv[2][-1]), regime)
        vector_code = coyote.vectorize()
        benchmark_name = f'tree_{regime}-{depth}'
        try:
            os.mkdir('outputs')
        except FileExistsError:
            pass
        
        try:
            os.mkdir(f'outputs/{benchmark_name}')
        except FileExistsError:
            pass

        open(f'outputs/{benchmark_name}/scal', 'w').write('\n'.join(scalar_code))
        open(f'outputs/{benchmark_name}/vec', 'w').write('\n'.join(map(str, vector_code)))
        print(f'Successfully compiled benchmark {benchmark_name}; outputs placed in "outputs/{benchmark_name}"!')
