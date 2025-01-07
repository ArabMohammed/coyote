import argparse
import os
import csv
import time
import evaluate
import subprocess
from evaluate import evaluate_scalar , evaluate_vector

depths = ['5', '10']
regimes = ['50-50', '100-50', '100-100']
depths = ['10']
regimes = ['100-100']

#parser = argparse.ArgumentParser()
#parser.add_argument('-d', '--depths', nargs='+', choices=depths, required=True)
#parser.add_argument('-r', '--regimes', nargs='+', choices=regimes, required=True)
#parser.add_argument('-i', '--iters', type=int, required=True)

#args = parser.parse_args()

with open('polynomial_benchmarks_evaluation.csv', mode='a', newline='') as csvfile:
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
###################################################################################
###################################################################################
iterations = 1 # 2
for regime in regimes:
    for depth in depths:
        for i in range(iterations):
            benchmark = f'tree_{regime}-{depth}_{i+1}'
            print(f'Benchmark {benchmark}...')
            ### start calculating compile time
            start_time = time.time()
            os.system(f'python3 numberTreeGenerator.py build {benchmark}')
            end_time = time.time()
            compilation_time = end_time - start_time
            ## end 
            os.system(f'python3 compile_to_bfv.py {benchmark}')
            num_multiplications_vec, num_additions_vec, num_substitutions_vec, num_rotations_vec, num_plain_multiplications_vec, max_depth_vec, max_multiplicative_depth_vec = evaluate_vector(benchmark)
            num_multiplications_scal, num_additions_scal, num_substitutions_scal, num_rotations_scal, num_plain_multiplications_scal, max_depth_scal, max_multiplicative_depth_scal = evaluate_scalar(benchmark)

            with open('polynomial_benchmarks_evaluation.csv', mode='a', newline='') as csvfile:
                csvwriter = csv.writer(csvfile)
                csvwriter.writerow(
                            [benchmark, compilation_time,
                            str(num_multiplications_scal)+" "+str(num_multiplications_vec), 
                            str(num_additions_scal)+" "+str(num_additions_vec), 
                            str(num_substitutions_scal)+" "+str(num_substitutions_vec), 
                            str(num_rotations_scal)+" "+str(num_rotations_vec), 
                            str(num_plain_multiplications_scal)+" "+str(num_plain_multiplications_vec), 
                            str(max_depth_scal)+" "+str(max_depth_vec), 
                            str(max_multiplicative_depth_scal)+" "+str(max_multiplicative_depth_vec)]
                            )
#############################################################################
###########################################################################
subprocess.run(['python3', 'build_and_run_all.py', '--iters=5', '--runs=5'])

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

with open('polynomial_benchmarks_evaluation.csv', mode='r', newline='') as infile:
    reader = csv.reader(infile)
    headers = next(reader)
    rows = list(reader)

if 'Run time' not in headers:
    headers.append('Run time')

# Update the benchmark evaluation CSV with the run times
with open('polynomial_benchmarks_evaluation.csv', mode='w', newline='') as outfile:
    writer = csv.writer(outfile)
    writer.writerow(headers)
    for row in rows:
        benchmark_name = row[0]
        if benchmark_name in run_times:
            row.append(run_times[benchmark_name])
        else:
            row.append('')
        writer.writerow(row)


# benchmarks = ['tree_100-100-5_1', 'tree_100-100-5_2', 'tree_100-100-5_3',
#               'tree_100-100-5_4', 'tree_100-100-5_5', 'tree_100-100-10_1',
#               'tree_100-100-10_2', 'tree_100-100-10_3', 'tree_100-100-10_4', 
#               'tree_100-100-10_5']

# benchmarks = ['tree_100-50-10_1',
#               'tree_100-50-10_2', 'tree_100-50-10_3', 'tree_100-50-10_4', 
#               'tree_100-50-10_5']

# # benchmarks = ['tree_50-50-5_1', 'tree_50-50-5_2', 'tree_50-50-5_3',
# #               'tree_50-50-5_4', 'tree_50-50-5_5', 'tree_50-50-10_1',
# #               'tree_50-50-10_2', 'tree_50-50-10_3', 'tree_50-50-10_4', 
# #               'tree_50-50-10_5']

# for benchmark in benchmarks:
#     os.system("python3 numberTreeGenerator.py build " + benchmark)
#     os.system("python3 compile_to_bfv.py " + benchmark)