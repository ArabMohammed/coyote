import argparse
import glob
import os
import shutil
import subprocess
import sys

def prepare_main_file(poly_modulus_degree):
    info1 = """
    #define _(x) #x
    #define __(x) _(x)

    #ifndef BENCHMARK_NAME 
    #define BENCHMARK_NAME aout 
    #endif

    #ifndef ITERATIONS
    #define ITERATIONS 1
    #endif

    #ifndef RUNS
    #define RUNS 1
    #endif

    #include "util.hpp"
    #include "vector.hpp"
    #include <chrono>
    #include <iostream>
    #include <fstream>
    #include <string>

    using _clock = std::chrono::high_resolution_clock;
    using ms = std::chrono::milliseconds;

    int main(int argc, char** argv)
    { 
        std::string csv_filename = __(BENCHMARK_NAME);
        csv_filename += ".csv";
        std::cout << "Running benchmark " << __(BENCHMARK_NAME) << "..." << std::endl;
        std::ofstream myfile(csv_filename);
        myfile << "VEC,ENC,RUN,ENC + RUN" << std::endl;
        for (int runs = 0; runs < RUNS; runs++) {
    """

    info2 = f"""        size_t poly_modulus_degree = {poly_modulus_degree};
    """

    info3 = """
            seal::EncryptionParameters params(seal::scheme_type::bfv);
            params.set_poly_modulus_degree(poly_modulus_degree);
            params.set_coeff_modulus(seal::CoeffModulus::BFVDefault(poly_modulus_degree));
            params.set_plain_modulus(seal::PlainModulus::Batching(poly_modulus_degree, 20));

            RuntimeContext info(params);

            VectorProgram vec(info);
            long long   vector_enc_time = 0,
                        vector_run_time = 0;
                        
            auto chkpoint = _clock::now();

            /****************************************************************************************/
            /****************************************************************************************/
            
            bool vector_program_success = true ;
            for (int i = 0; i < ITERATIONS; i++)
            {
                chkpoint = _clock::now(); 
                vec.setup();
                vector_enc_time += std::chrono::duration_cast<ms>(_clock::now() - chkpoint).count();
                chkpoint = _clock::now();
                vector_program_success = vector_program_success && vec.run();
                vector_run_time += std::chrono::duration_cast<ms>(_clock::now() - chkpoint).count();
            }
            vector_run_time = vector_run_time / ITERATIONS;
            vector_enc_time = vector_enc_time / ITERATIONS;
            if(vector_program_success){
                std::cout << "Vector took (enc, run, enc + run) = (" << vector_enc_time << ", " << vector_run_time << ", " << vector_enc_time + vector_run_time << ")" << std::endl;
            }else{
                std::cout << "Vector took (enc, run, enc + run) = (Error, Error, Error)" << std::endl;
            }

            /***********************************************************************************************/
            /***********************************************************************************************/
            if (!vector_program_success) {
                myfile << "v,ErrorV,ErrorV,ErrorV" << std::endl;
            } else {
                myfile << "v," << vector_enc_time << "," << vector_run_time << "," << vector_enc_time + vector_run_time << std::endl;
            }
        }

        myfile.close();
        return 0;
    }
    """

    # Concatenate
    cpp_code = info1 + info2 + info3

    return cpp_code

#####################################################################################
def ensure_exists(dirname):
    os.makedirs(dirname, exist_ok=True)

parser = argparse.ArgumentParser()
parser.add_argument('-r', '--runs', type=int, default=1)
parser.add_argument('-i', '--iters', type=int, default=1)
parser.add_argument('-p', '--poly_modulus_degres', type=int, default=2048)
parser.add_argument('-b', '--benchmarks', type=str, help='Comma-separated list of benchmarks (without extension)')
###############################
args = parser.parse_args()
##########################
poly_modulus_degres = args.poly_modulus_degres
with open("bfv_backend/main.cpp","w") as f : 
    f.write(prepare_main_file(poly_modulus_degres))
##########################
if args.benchmarks:
    benchmarks = args.benchmarks.split(',')
    print(f"==> build_run_all : benchmarks to run : {benchmarks}")
else:
    benchmarks = glob.glob('*', root_dir='bfv_backend/coyote_out')
    print(f"==> build_run_all : defaulted to run benchmarks  : {benchmarks}")

##############################
##############################
subprocess.Popen(['cmake', '..', f'-DRUNS={args.runs}', f'-DITERATIONS={args.iters}'], cwd='bfv_backend/build', stdout=sys.stdout, stderr=sys.stderr).wait()
subprocess.Popen('make', cwd='bfv_backend/build', stdout=sys.stdout, stderr=sys.stderr).wait()


ensure_exists('bfv_backend/csvs')
#benchmarks=['sort_3_packed_fully','max_5_packed_fully']
for benchmark in benchmarks:
    subprocess.Popen(f'../build/{benchmark}', cwd='bfv_backend/csvs', stdout=sys.stdout, stderr=sys.stderr).wait()
    
# TODO: clean these up by benchmark name and size
csv_files = glob.glob('bfv_backend/csvs/*.csv')
for csv in csv_files: 
    parts = os.path.splitext(os.path.basename(csv))[0].split('_')
    if not parts[0].startswith('tree'):
        repl = parts[-1]
        size = parts[-2] 
        name = '_'.join(parts[:-2])
        ensure_exists(f'bfv_backend/csvs/{name}/{size}')
        shutil.copy(csv, f'bfv_backend/csvs/{name}/{size}/{"_".join(parts)}.csv')
    # os.rename(csv, f'bfv_backend/csvs/{name}/{size}/{"_".join(parts)}.csv')
