import os , shutil
from coyote import *
import time
import csv
import subprocess
from evaluate import * 
import numpy as np 
import numbers 
import statistics 

###################################################################
###################################################################
def cond(b, true, false):
    return b * true + (Var('1') + b) * false
 
def shift_down(image):
    return [[Var('0')] * 16] + image[:-1] 

def shift_up(image): 
    return image[1:] + [[Var('0')] * 16]

def shift_left(image):
    return [row[1:] + [Var('0')] for row in image]

def shift_right(image):
    return [[Var('0')] + row[:-1] for row in image]

def matrix_addition(matrix1, matrix2):    
    return [[matrix1[i][j] + (matrix2[i][j]) for j in range(16)] for i in range(16)]


def all_numbers(lst):
    return all(isinstance(x, numbers.Number) for x in lst)
###################################################################
###################################################################
coyote = coyote_compiler()
"""
## for comparaison with Coyote, all benchamrks are Fully replicated  
@coyote.define_circuit(v1=vector(4, replicate=True), v2=vector(4, replicate=True))
def dot_product_4x4_fully(v1, v2): 
    return recursive_sum([a * b for a, b in zip(v1, v2)])
#################################################################################
#################################################################################
@coyote.define_circuit(a=vector(4,replicate=True), b=vector(4,replicate=True))
def l2_distance_4(a, b):
    return recursive_sum([(x - y) * (x - y) for x, y in zip(a, b)])

#################################################################################
#################################################################################
@coyote.define_circuit(a=vector(4,replicate=True), b=vector(4,replicate=True))
def hamming_distance_4(a, b):
    return [x + y - Var('2')*(x * y) for x, y in zip(a, b)]
#################################################################################
#################################################################################
@coyote.define_circuit(c0=vector(4,replicate=True), c1=vector(4,replicate=True), c2=vector(4,replicate=True), c3=vector(4,replicate=True), c4=vector(4,replicate=True))
def poly_reg_4(c0, c1, c2, c3, c4):
    return [c1[i] - (c0[i] * c0[i] * c4[i] + c0[i] * c3[i] + c2[i]) for i in range(4)]
"""
#################################################################################
#################################################################################
@coyote.define_circuit(c0=vector(4,replicate=True), c1=vector(4,replicate=True), c2=vector(4,replicate=True), c3=vector(4,replicate=True))
def linear_reg_4(c0, c1, c2, c3):
    return [c1[i] - (c2[i] * c0[i]) - c3[i] for i in range(4)]

#################################################################################
########################## Configuration ########################################
Execute_code = True 
iterations = 1  
poly_modulus_degres = 16384
#################################################################################
###########################Prepare Evaluation CSV File############################
operations = ["add", "sub", "multiply_plain", "rotate_rows", "multiply"]
infos = ["benchmark"]
additional_infos =["Depth", "Multplicative Depth","compile_time (s)", "execution_time (s)"]
infos.extend(operations)
infos.extend(additional_infos)
with open('benchmarks_evaluation.csv', mode='a', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    csvwriter.writerow(infos)

#########################################################################
########################## Run General benchmarks########################
#########################################################################
directory_path = 'bfv_backend/coyote_out'
functions_to_compile = [func.__name__ for func in coyote.func_signatures]
benchmarks_str_argument = ','.join(functions_to_compile)
print(f"||| General benchs ====> {benchmarks_str_argument} \n")
for func in coyote.func_signatures: 
    print(f"Compile function : {func.__name__}")
    benchmark_name = func.__name__
    benchmark_dir = os.path.join(directory_path, benchmark_name)
    operation_stats = {
            "add": [], "sub": [], "multiply_plain": [], "rotate_rows": [],
            "multiply": [], "Depth": [], "Multiplicative Depth": [],
            "compile_time (s)": [], "execution_time (s)": [], 
            }
    for counter in range(iterations):       
        start_time = time.time()
        scalar_code = coyote.instantiate(benchmark_name)
        vector_code = list(map(str, coyote.vectorize()))
        end_time = time.time()
        compilation_time = end_time - start_time
        try:
            os.mkdir('outputs')
        except FileExistsError:
            pass

        try:
            os.mkdir(f'outputs/{benchmark_name}')
        except FileExistsError: 
            pass 
        open(f'outputs/{benchmark_name}/scal', 'w').write('\n'.join(scalar_code))
        open(f'outputs/{benchmark_name}/vec', 'w').write('\n'.join(vector_code))
        print(f'Successfully compiled benchmark {benchmark_name}; outputs placed in outputs/{benchmark_name}!')
        # Start the subprocess and capture stdout and stderr
        print("compile to bfv the benchmark :")
        result = subprocess.Popen(['python3', 'compile_to_bfv.py', benchmark_name],
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                text=True)
        # Wait for the subprocess to complete and capture output
        stdout, stderr = result.communicate()
        # Display captured output
        print(stdout)  # Standard output
        num_multiplications_vec, num_additions_vec, num_substitutions_vec, num_rotations_vec, num_plain_multiplications_vec, max_depth_vec, max_multiplicative_depth_vec = evaluate_vector(benchmark_name)    
        ################
        operation_stats["add"].append(num_additions_vec)
        operation_stats["sub"].append(num_substitutions_vec)
        operation_stats["multiply_plain"].append(num_plain_multiplications_vec)
        operation_stats["rotate_rows"].append(num_rotations_vec)
        operation_stats["multiply"].append(num_multiplications_vec)
        operation_stats["Depth"].append(max_depth_vec)
        operation_stats["Multiplicative Depth"].append(max_multiplicative_depth_vec)
        operation_stats["compile_time (s)"].append(compilation_time)
       
    ################################################################
    with open('benchmarks_evaluation.csv', mode='a', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        row=[benchmark_name]
        keys = list(operation_stats.keys())[:-1]
        print(keys)
        for key in keys:
            values = operation_stats[key]
            print(f"==> key : {key} , values : {values}")
            if len(values) > 0 and all_numbers(values): 
                result = statistics.median(values) 
                if key == "compile_time (s)" :
                    result = format(result, ".3f")
                row.append(str(result)) 
            else : 
                row.append(" ") 
        csvwriter.writerow(row)

##########################################################################
########################### Run polynomials ##############################
##########################################################################
depths = ['5']
regimes = ['50-50']
for regime in regimes:
    for depth in depths:
        operation_stats = {
            "add": [], "sub": [], "multiply_plain": [], "rotate_rows": [],
            "multiply": [], "Depth": [], "Multiplicative Depth": [],
            "compile_time (s)": [], "execution_time (s)": [], 
        } 
        bench_name = f'tree_{regime}-{depth}'
        benchmarks_str_argument+=","+bench_name
        for i in range(iterations):
            cases = {
                '50-50':'0',
                '100-50':'1',
                '100-100':'2'
            }
            benchmark_name = f'tree_{cases[regime]}-{depth}_{i+1}'
            print(f'Benchmark {benchmark_name}...')
            ### start calculating compile time
            start_time = time.time()
            os.system(f'python3 numberTreeGenerator.py build {benchmark_name}')
            end_time = time.time()
            compilation_time = end_time - start_time
            ## end 
            benchmark_name = f'tree_{regime}-{depth}'
            os.system(f'python3 compile_to_bfv.py {benchmark_name}')
            
            num_multiplications_vec, num_additions_vec, num_substitutions_vec, num_rotations_vec, num_plain_multiplications_vec, max_depth_vec, max_multiplicative_depth_vec = evaluate_vector(benchmark_name)
            ################
            operation_stats["add"].append(num_additions_vec)
            operation_stats["sub"].append(num_substitutions_vec)
            operation_stats["multiply_plain"].append(num_plain_multiplications_vec)
            operation_stats["rotate_rows"].append(num_rotations_vec)
            operation_stats["multiply"].append(num_multiplications_vec)
            operation_stats["Depth"].append(max_depth_vec)
            operation_stats["Multiplicative Depth"].append(max_multiplicative_depth_vec)
            operation_stats["compile_time (s)"].append(compilation_time)
        ################################################################
        with open('benchmarks_evaluation.csv', mode='a', newline='') as csvfile:
            csvwriter = csv.writer(csvfile)
            row=[bench_name]
            keys = list(operation_stats.keys())[:-1]
            for key in keys:
                values = operation_stats[key]
                if len(values) > 0 and all_numbers(values): 
                    result = statistics.median(values)
                    if key == "compile_time (s)" :
                        result = format(result, ".3f") 
                    row.append(str(result)) 
                else : 
                    row.append(" ") 
            csvwriter.writerow(row)

#############################################################################
#############################################################################
print(f"||| Final ====> {benchmarks_str_argument} \n")
if Execute_code : 
    process = subprocess.Popen(['python3', 'build_and_run_all.py', f'--iters={iterations}', '--runs=1',f'--poly_modulus_degres={poly_modulus_degres}',f'--benchmarks={benchmarks_str_argument}'])
    process.wait()
    csv_directory = 'bfv_backend/csvs'
    run_times = {} 
    for filename in os.listdir(csv_directory):
        if filename.endswith('.csv'):
            benchmark_name = filename[:-4]  # Remove the '.csv' extension to get the benchmark name
            file_path = os.path.join(csv_directory, filename)

            # Read the second row and fourth column from the CSV file
            with open(file_path, mode='r', newline='') as csvfile:
                reader = csv.reader(csvfile)
                rows = list(reader)
                print(f"benchmark : {benchmark_name} infos ===>{rows}")
                if len(rows) > 1 and len(rows[1]) > 3:  # Ensure there is a second row and fourth column
                    run_time = rows[1][3]  # Get the value from the second row and fourth column
                    try:
                        run_time = float(run_time)/1000
                        run_time = format(run_time, ".3f") 
                        run_times[benchmark_name] = run_time
                    except ValueError:                      
                        run_times[benchmark_name] = run_time
                else :  
                    run_times[benchmark_name] = "ErrorG"

    with open('benchmarks_evaluation.csv', mode='r', newline='') as infile:
        reader = csv.reader(infile)
        headers = next(reader)
        rows = list(reader) 
    # Update the benchmark evaluation CSV with the run times
    with open('benchmarks_evaluation.csv', mode='w', newline='') as outfile:
        writer = csv.writer(outfile)
        writer.writerow(headers)
        print(f"\n===> Obtained runtimes : {run_times}\n")
        for row in rows:
            benchmark_name = row[0]
            if benchmark_name in run_times:
                row.append(run_times[benchmark_name])
            else:
                row.append('')
            writer.writerow(row)
