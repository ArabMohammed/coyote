
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
        myfile << "VEC,ENC,RUN,ENC + RUN,Remaining_noise_budget" << std::endl;
        for (int runs = 0; runs < RUNS; runs++) {
            size_t poly_modulus_degree = 16384;
    
            seal::EncryptionParameters params(seal::scheme_type::bfv);
            params.set_poly_modulus_degree(poly_modulus_degree);
            params.set_coeff_modulus(seal::CoeffModulus::BFVDefault(poly_modulus_degree));
            params.set_plain_modulus(seal::PlainModulus::Batching(poly_modulus_degree, 20));

            RuntimeContext info(params);

            VectorProgram vec(info);
            long  vector_enc_time = 0;
            long  vector_run_time = 0;
            long  Remaining_noise_budget = 0;
                        
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
                Remaining_noise_budget += vec.run();
                vector_run_time += std::chrono::duration_cast<ms>(_clock::now() - chkpoint).count();
            }
            vector_run_time = vector_run_time / ITERATIONS;
            vector_enc_time = vector_enc_time / ITERATIONS;
            Remaining_noise_budget = Remaining_noise_budget / ITERATIONS;
            if(Remaining_noise_budget>0){
                std::cout << "Vector took (enc, run, enc + run, Remaining_noise_budget) = (" << vector_enc_time << ", " << vector_run_time << ", " << vector_enc_time + vector_run_time <<", "<<Remaining_noise_budget<<")" << std::endl;
            }else{
                std::cout << "Vector took (enc, run, enc + run, Remaining_noise_budget) = (Error, Error, Error, 0)" << std::endl;
            }

            /***********************************************************************************************/
            /***********************************************************************************************/
            if (Remaining_noise_budget<=0) {
                myfile << "v,ErrorV,ErrorV,ErrorV,0" << std::endl;
            } else {
                myfile << "v," << vector_enc_time << "," << vector_run_time << "," << vector_enc_time + vector_run_time << "," << Remaining_noise_budget << std::endl;
            }
        }

        myfile.close();
        return 0;
    }
    