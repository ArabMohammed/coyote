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
@coyote.define_circuit(c0=vector(4,replicate=True), c1=vector(4,replicate=True), c2=vector(4,replicate=True), c3=vector(4,replicate=True), c4=vector(4,replicate=True))
def poly_reg_4(c0, c1, c2, c3, c4):
    return [c1[i] - (c0[i] * c0[i] * c4[i] + c0[i] * c3[i] + c2[i]) for i in range(4)]

#################################################################################
#################################################################################
@coyote.define_circuit(c0=vector(4,replicate=True), c1=vector(4,replicate=True), c2=vector(4,replicate=True), c3=vector(4,replicate=True))
def linear_reg_4(c0, c1, c2, c3):
    return [c1[i] - (c2[i] * c0[i]) - c3[i] for i in range(4)]

#################################################################################
#################################################################################
@coyote.define_circuit(a=vector(4,replicate=True), b=vector(4,replicate=True))
def hamming_distance_4(a, b):
    return [x + y - Var('2')*(x * y) for x, y in zip(a, b)]

#################################################################################
#################################################################################
@coyote.define_circuit(image = matrix(4,4,replicate=True))
def box_blur_4x4(image):
    height, width = 4,4
    result = np.empty((height, width), dtype=object)
    result[:] = Var('0')
    ####################
    for i in range(height):
        for j in range(width):
            for ki in range(-1,2): 
                for kj in range(-1,2):
                    ni = i + ki
                    nj = j + kj
                    #Ensure the indices are within the image bounds
                    if (ni >= 0 and ni < height and nj >= 0 and nj < width):
                        result[i, j] = result[i, j] + image[ni][nj]
    return [result[i][j] for i in range(height) for j in range(width)]

#################################################################################
#################################################################################
@coyote.define_circuit(image = matrix(4,4,replicate=True))
def gx_kernel_4x4(image):
    height, width = 4,4  
    result = np.zeros_like(image) 
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))

    for i in range(1, height + 1):
        for j in range(1, width + 1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            
            top_sum = top_row[0]*Var('-1')  +   top_row[2]*Var('1')
            curr_sum = Var('-2')*curr_row[0] + Var('2') * curr_row[2]
            bottom_sum = bottom_row[0]*Var('-1') + bottom_row[2]*Var('1')

            result[i-1, j-1] = top_sum + curr_sum + bottom_sum

    return [result[i][j] for i in range(height) for j in range(width)]

#################################################################################
#################################################################################
@coyote.define_circuit(image = matrix(4,4,replicate=True))
def gy_kernel_4x4(image):
    height, width = 4,4
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    for i in range(1, height + 1):
        for j in range(1, width + 1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            
            top_sum = Var('-1')*top_row[0] + Var('-2')*top_row[1]  + Var('-1')*top_row[2]
            bottom_sum = Var('1')*bottom_row[0] + Var('2')*bottom_row[1] + Var('1')*bottom_row[2]

            result[i-1, j-1] = top_sum + bottom_sum

    return [result[i][j] for i in range(height) for j in range(width)]

#################################################################################
#################################################################################
@coyote.define_circuit(image = matrix(4,4,replicate=True))
def roberts_cross_4x4(image):
    height, width = 4,4
    ## add padding to the image
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    gx_kernel = [[Var('1'),Var('0')],[Var('0'),Var('-1')]]
    gy_kernel = [[Var('0'),Var('1')],[Var('-1'),Var('0')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2] 
            Gx = curr_row[1] + bottom_row[2] 
            # Apply Kernel 2 (Gy)
            Gy = curr_row[2] + bottom_row[1]    
            # Compute the gradient magnitude and store in the output array (clipped to 255)
            result[i-1][j-1] = Gx*Gx + Gy*Gy
    return [result[i][j] for i in range(height) for j in range(width)]

#################################################################################
#################################################################################
@coyote.define_circuit(image = matrix(4,4,replicate=True))
def sobel_4x4(image):
    height, width = 4,4
    result = np.zeros_like(image)
    padded_image = np.pad(image, pad_width=1, mode='constant', constant_values=Var('0'))
    gx_output = result 
    gy_output = result
    gx_kernel = [[Var('-1'),Var('0'),Var('1')],[Var('-2'),Var('0'),Var('2')],[Var('-1'),Var('0'),Var('1')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            #Traverse the kernel window centered around (i, j)
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            top_sum = Var('-1')*top_row[0] + top_row[2]
            curr_sum = Var('-2')*curr_row[0] + Var('2') * curr_row[2]
            bottom_sum = Var('-1')*bottom_row[0] + bottom_row[2]
            gx_output[i-1, j-1] = top_sum + curr_sum + bottom_sum
    ###########################
    gy_kernel = [[Var('-1'),Var('-2'),Var('-1')],[Var('0'),Var('0'),Var('0')],[Var('1'),Var('2'),Var('1')]]
    for i in range(1,height+1):
        for j in range(1,width+1):
            top_row = padded_image[i-1, j-1:j+2]
            curr_row = padded_image[i, j-1:j+2]
            bottom_row = padded_image[i+1, j-1:j+2]
            top_sum = Var('-1')*top_row[0] + Var('-2')*top_row[1]  + Var('-1')*top_row[2]
            bottom_sum = bottom_row[0] + Var('2')*bottom_row[1] + bottom_row[2]
            gy_output[i-1, j-1] = top_sum + bottom_sum
    ########################            
    for i in range(height):
        for j in range(width):
            result[i][j]= gx_output[i][j] * gx_output[i][j] + gy_output[i][j] * gy_output[i][j]
    return [result[i][j] for i in range(height) for j in range(width)]

#################################################################################
#################################################################################
@coyote.define_circuit(a=matrix(4, 4, replicate=True), b=matrix(4, 4, replicate=True))
def matmul_4x4_fully(a, b):
    return [recursive_sum([a[i][k] * b[k][j] for k in range(len(a))]) for i in range(len(a)) for j in range(len(a))]

#################################################################################
#################################################################################
@coyote.define_circuit(cs=vector(3, replicate=True), os=vector(6, replicate=True))
def sort_3_packed_fully(cs, os):
    return cond(cs[0], 
                (cond(cs[1], 
                    os[0],
                    cond(cs[2], os[1], os[4]))), 
                (cond(cs[2],
                    os[2],
                    cond(cs[1], os[3], os[5]))))
#####################################################
@coyote.define_circuit(cs=vector(6, replicate=True), os=vector(24, replicate=True))
def sort_4_packed_fully(cs, os):
    return cond(
        cs[0],  # c12 (v1 ≤ v2)
        # Case 1: v1 ≤ v2
        cond(
            cs[2],  # c13 (v1 ≤ v3)
            # Subcase 1.1: v1 ≤ v2 and v1 ≤ v3 → v1 is smallest
            cond(
                cs[3],  # c23 (v2 ≤ v3)
                # Subcase 1.1.1: v1 ≤ v2 ≤ v3 → insert v4
                cond(
                    cs[1],  # c14 (v1 ≤ v4)
                    cond(
                        cs[4],  # c24 (v2 ≤ v4)
                        cond(cs[5], os[0], os[1]),  # o1234 or o1243
                        cond(cs[1], os[4], os[18])   # o1423 or o4123
                    ),
                    os[0]  # Fallback (o1234)
                ),
                # Subcase 1.1.2: v1 ≤ v3 ≤ v2 → insert v4
                cond(
                    cs[1],  # c14 (v1 ≤ v4)
                    cond(
                        cs[5],  # c34 (v3 ≤ v4)
                        cond(cs[4], os[2], os[3]),  # o1324 or o1342
                        cond(cs[1], os[5], os[19])   # o1432 or o4132
                    ),
                    os[2]  # Fallback (o1324)
                )
            ),
            # Subcase 1.2: v1 ≤ v2 and v3 ≤ v1 → v3 ≤ v1 ≤ v2 → insert v4
            cond(
                cs[5],  # c34 (v3 ≤ v4)
                cond(
                    cs[1],  # c14 (v1 ≤ v4)
                    cond(cs[4], os[12], os[13]),  # o3124 or o3142
                    cond(cs[5], os[16], os[22])    # o3412 or o4312
                ),
                os[12]  # Fallback (o3124)
            )
        ),
        # Case 2: v2 ≤ v1
        cond(
            cs[3],  # c23 (v2 ≤ v3)
            # Subcase 2.1: v2 ≤ v1 and v2 ≤ v3 → v2 is smallest
            cond(
                cs[2],  # c13 (v1 ≤ v3)
                # Subcase 2.1.1: v2 ≤ v1 ≤ v3 → insert v4
                cond(
                    cs[4],  # c24 (v2 ≤ v4)
                    cond(
                        cs[1],  # c14 (v1 ≤ v4)
                        cond(cs[5], os[6], os[7]),  # o2134 or o2143
                        cond(cs[4], os[10], os[20])  # o2413 or o4213
                    ),
                    os[6]  # Fallback (o2134)
                ),
                # Subcase 2.1.2: v2 ≤ v3 ≤ v1 → insert v4
                cond(
                    cs[4],  # c24 (v2 ≤ v4)
                    cond(
                        cs[5],  # c34 (v3 ≤ v4)
                        cond(cs[1], os[8], os[9]),  # o2314 or o2341
                        cond(cs[4], os[11], os[21])  # o2431 or o4231
                    ),
                    os[8]  # Fallback (o2314)
                )
            ),
            # Subcase 2.2: v2 ≤ v1 and v3 ≤ v2 → v3 ≤ v2 ≤ v1 → insert v4
            cond(
                cs[5],  # c34 (v3 ≤ v4)
                cond(
                    cs[4],  # c24 (v2 ≤ v4)
                    cond(cs[1], os[14], os[15]),  # o3214 or o3241
                    cond(cs[5], os[17], os[23])    # o3421 or o4321
                ),
                os[14]  # Fallback (o3214)
            )
        )
    )

##################################################################################
##################################################################################
@coyote.define_circuit(cs=vector(3, replicate=True), os=vector(3, replicate=True))
def max_3_packed_fully(cs, os):
    return cond(cs[0], 
                cond(cs[1], os[0],os[2]),    
                cond(cs[2],os[1],os[2])
                )
#####################################################
@coyote.define_circuit(cs=vector(6, replicate=True), os=vector(4, replicate=True))
def max_4_packed_fully(cs, os):
    return cond(cs[0], 
                cond(cs[1], cond(cs[2],os[0],os[3]),cond(cs[4],os[2],os[3])),    
                cond(cs[3],cond(cs[5],os[1],os[3],),cond(cs[4],os[2],os[3],))
                )
#####################################################
@coyote.define_circuit(cs=vector(10, replicate=True), os=vector(5, replicate=True))
def max_5_packed_fully(cs, os):
    return cond(cs[0], 
                (cond(cs[1], 
                    cond(cs[3],
                        cond(cs[6], os[0], os[4]),
                        cond(cs[7], os[3], os[4])),
                    cond(cs[4],
                        cond(cs[8], os[2], os[4]),
                        cond(cs[7], os[3], os[4])))),
                    
                (cond(cs[2],
                    cond(cs[5],
                        cond(cs[9], os[1], os[4]),
                        cond(cs[7], os[3], os[4])),
                    cond(cs[4],
                        cond(cs[8], os[2], os[4]),
                        cond(cs[7], os[3], os[4])))))

#################################################################################
########################## Configuration ########################################
Execute_code = True 
iterations = 3   
poly_modulus_degres = 16384
#################################################################################
###############################Prepare Evaluation CSV File##############################################
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
depths = ['5','10']
regimes = ['50-50','100-50','100-100']
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
            print(f"keys : {keys}")
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
        print(f"\n===> Obtained runtimes in seconds: {run_times}\n")
        for row in rows:
            benchmark_name = row[0]
            if benchmark_name in run_times:
                row.append(run_times[benchmark_name])
            else:
                row.append('')
            writer.writerow(row)
