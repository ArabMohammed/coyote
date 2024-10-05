import os
from coyote import *
import time
import csv
import subprocess
from evaluate import *
import numpy as np

def shift_down(image):
    return [[Var('0')] * 4] + image[:-1]

def shift_up(image):
    return image[1:] + [[Var('0')] * 4]

def shift_left(image):
    return [row[1:] + [Var('0')] for row in image]

def shift_right(image):
    return [[Var('0')] + row[:-1] for row in image]

def matrix_addition(matrix1, matrix2):    
    return [[matrix1[i][j] + (matrix2[i][j]) for j in range(4)] for i in range(4)]

coyote = coyote_compiler()

@coyote.define_circuit(image = matrix(4,4))
def gx_kernel_4x4(image):
    height, width = 4,4
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))

    for i in range(1, height + 1):
        for j in range(1, width + 1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            
            top_sum = top_row[0] +   top_row[2]
            curr_sum = Var('2')*curr_row[0] + Var('2') * curr_row[2]
            bottom_sum = bottom_row[0] + bottom_row[2]

            result[i-1, j-1] = top_sum + curr_sum + bottom_sum

    return [result[i][j] for i in range(4) for j in range(4)]

@coyote.define_circuit(a=matrix(4, 4), b=matrix(4, 4))
def matmul_4x4_un(a, b):
    return [recursive_sum([a[i][k] * b[k][j] for k in range(len(a))]) for i in range(len(a)) for j in range(len(a))]

@coyote.define_circuit(a=vector(4), b=vector(4))
def l2_distance_4(a, b):
    return recursive_sum([(x - y) * (x - y) for x, y in zip(a, b)])

@coyote.define_circuit(c0=vector(4), c1=vector(4), c2=vector(4), c3=vector(4), c4=vector(4))
def poly_reg_4(c0, c1, c2, c3, c4):
    return [c1[i] - (c0[i] * c0[i] * c4[i] + c0[i] * c3[i] + c2[i]) for i in range(4)]

@coyote.define_circuit(c0=vector(4), c1=vector(4), c2=vector(4), c3=vector(4))
def linear_reg_4(c0, c1, c2, c3):
    return [c1[i] - (c2[i] * c0[i]) - c3[i] for i in range(4)]

@coyote.define_circuit(image = matrix(4,4))
def box_blur_4x4(image):
    top_row = shift_up(image)
    bottom_row = shift_down(image)

    top_sum = matrix_addition(matrix_addition(shift_left(top_row), top_row), shift_right(top_row))
    curr_sum = matrix_addition(matrix_addition(shift_left(image), image), shift_right(image))
    bottom_sum = matrix_addition(matrix_addition(shift_left(bottom_row), bottom_row), shift_right(bottom_row))

    result = matrix_addition(matrix_addition(top_sum, curr_sum), bottom_sum)

    return [result[i][j] for i in range(4) for j in range(4)]

with open('benchmarks_evaluation.csv', mode='a', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    # Write the header
    csvwriter.writerow(['Benchmark name', 'Compile time',
                        'multiplications',
                        'additions',
                        'substitutions',
                        'rotations',
                        'scalar/plain multiplications',
                        'Depth',
                        'Multiplicative depth'
                        ])

directory_path = 'bfv_backend/coyote_out'
for func in coyote.func_signatures:
    print(func.__name__)
    benchmark_name = func.__name__
    benchmark_dir = os.path.join(directory_path, benchmark_name)

    if not os.path.exists(benchmark_dir):
        start_time = time.time()
        scalar_code = coyote.instantiate(benchmark_name)
        vector_code = list(map(str, coyote.vectorize()))

        end_time = time.time()
        compilation_time = end_time - start_time
        print(f'Compilation time: {compilation_time:.2f} seconds')
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

        subprocess.run(['python3', 'compile_to_bfv.py', benchmark_name])
        
        num_multiplications_vec, num_additions_vec, num_substitutions_vec, num_rotations_vec, num_plain_multiplications_vec, max_depth_vec, max_multiplicative_depth_vec = evaluate_vector(benchmark_name)
        num_multiplications_scal, num_additions_scal, num_substitutions_scal, num_rotations_scal, num_plain_multiplications_scal, max_depth_scal, max_multiplicative_depth_scal = evaluate_scalar(benchmark_name)

        with open('benchmarks_evaluation.csv', mode='a', newline='') as csvfile:
            csvwriter = csv.writer(csvfile)
            csvwriter.writerow(
                            [benchmark_name, compilation_time,
                            str(num_multiplications_scal)+" "+str(num_multiplications_vec), 
                            str(num_additions_scal)+" "+str(num_additions_vec), 
                            str(num_substitutions_scal)+" "+str(num_substitutions_vec), 
                            str(num_rotations_scal)+" "+str(num_rotations_vec), 
                            str(num_plain_multiplications_scal)+" "+str(num_plain_multiplications_vec), 
                            str(max_depth_scal)+" "+str(max_depth_vec), 
                            str(max_multiplicative_depth_scal)+" "+str(max_multiplicative_depth_vec)]
                            )

###########################################################################
subprocess.run(['python3', 'build_and_run_all.py', '--iters=1', '--runs=1'])

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
            if len(rows) > 1 and len(rows[1]) > 3:  # Ensure there is a second row and fourth column
                run_time = str(rows[1][7])+" "+str(rows[1][3])  # Get the value from the second row and fourth column
                run_times[benchmark_name] = run_time

with open('benchmarks_evaluation.csv', mode='r', newline='') as infile:
    reader = csv.reader(infile)
    headers = next(reader)
    rows = list(reader)

if 'Run time' not in headers:
    headers.append('Run time')

# Update the benchmark evaluation CSV with the run times
with open('benchmarks_evaluation.csv', mode='w', newline='') as outfile:
    writer = csv.writer(outfile)
    writer.writerow(headers)
    for row in rows:
        benchmark_name = row[0]
        if benchmark_name in run_times:
            row.append(run_times[benchmark_name])
        else:
            row.append('')
        writer.writerow(row)


