
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

    #include "scalar.hpp"
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
        myfile << "VEC,ENC,RUN,ENC + RUN,SCAL,ENC,RUN,ENC + RUN," << std::endl;
        for (int runs = 0; runs < RUNS; runs++) {

            size_t poly_modulus_degree = 16384;

            seal::EncryptionParameters params(seal::scheme_type::bfv);
            params.set_poly_modulus_degree(poly_modulus_degree);
            params.set_coeff_modulus(seal::CoeffModulus::BFVDefault(poly_modulus_degree));
            params.set_plain_modulus(seal::PlainModulus::Batching(poly_modulus_degree, 20));

            RuntimeContext info(params);

            ScalarProgram scal(info);
            VectorProgram vec(info);
            long long   vector_enc_time = 0,
                        vector_run_time = 0,
                        scalar_enc_time = 0,
                        scalar_run_time = 0;
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
            vector_run_time = vector_run_time / ITERATIONS ;
            vector_enc_time = vector_enc_time / ITERATIONS ;
            if(vector_program_success){
                std::cout << "Vector took (enc, run, enc + run) = (" << vector_enc_time << ", " << vector_run_time << ", " << vector_enc_time + vector_run_time << ")" << std::endl;
            }else{
                std::cout << "Vector took (enc, run, enc + run) = (Error, Error, Error)" << std::endl;
            }
            
            /******************************************************************************************/
            /******************************************************************************************/
            
            bool scalar_program_success = true ;
            for (int i = 0; i < ITERATIONS; i++)
            {
                chkpoint = _clock::now();
                scal.setup();
                scalar_enc_time += std::chrono::duration_cast<ms>(_clock::now() - chkpoint).count();
                chkpoint = _clock::now();
                scalar_program_success = scalar_program_success && scal.run();
                scalar_run_time += std::chrono::duration_cast<ms>(_clock::now() - chkpoint).count();
            }
            scalar_run_time = scalar_run_time / ITERATIONS ;
            scalar_enc_time = scalar_enc_time / ITERATIONS ;
            if(scalar_program_success){
                std::cout << "Scalar took (enc, run, enc + run) = (" << scalar_enc_time << ", " << scalar_run_time << ", "<< scalar_enc_time + scalar_run_time << ")" << std::endl;
            }else{
                std::cout << "Scalar took (enc, run, enc + run) = (Error, Error, Error)" << std::endl;
            }
            /***********************************************************************************************/
            /***********************************************************************************************/
            if (!scalar_program_success && !vector_program_success) {
                myfile << "v,ErrorV,ErrorV,ErrorV,s,ErrorS,ErrorS,ErrorS," << std::endl;
            } else if (!vector_program_success) {
                myfile << "v,ErrorV,ErrorV,ErrorV,s,"<< scalar_enc_time << "," << scalar_run_time << ","<< scalar_enc_time + scalar_run_time << "," << std::endl;
            } else if (!scalar_program_success) {
                myfile<<"v," << vector_enc_time << "," << vector_run_time << ","<<vector_enc_time + vector_run_time<< ",s,ErrorS,ErrorS,ErrorS," << std::endl;
            } else {
                myfile << "v," << vector_enc_time << "," << vector_run_time << ","<< vector_enc_time + vector_run_time<< ",s," << scalar_enc_time << "," << scalar_run_time << ","<< scalar_enc_time + scalar_run_time << "," << std::endl;
            }
        }
        myfile.close();
        return 0;
    }
    