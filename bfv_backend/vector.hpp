#include "util.hpp"
#include <iostream>
struct VectorProgram
{

    RuntimeContext &info;

    ctxt computation(std::vector<ctxt>, std::map<std::string, ptxt> bits, RuntimeContext &info);

    std::map<std::string, ptxt> make_bits(RuntimeContext &info);
    std::vector<ctxt> initialize_temps(RuntimeContext &info);

    std::map<std::string, ptxt> bits;
    std::vector<ctxt> temps;

    VectorProgram(RuntimeContext &info) : info(info){

    }

    void setup()
    {
        bits = make_bits(info);
        temps = initialize_temps(info);
    }

    bool run()
    { 
        bool success = true ;
        ctxt result = computation(temps,bits,info);
        std::cout<<"Checking the integrity of result returned by vector code \n";
        int L = info.context->first_context_data()->parms().coeff_modulus().size();
        std::cout<< "output ciphertexts info (L=" << L - 1 << ")\n";
        std::cout<< "id: level, remaining_noise_budget, actual_noise_upper_bound (maybe mod_switch was used to sacrifice some "
                "noise budget)\n"; 
        int init_noise_budget = info.context->first_context_data()->total_coeff_modulus_bit_count() -
                                info.context->first_context_data()->parms().plain_modulus().bit_count();
        int level = info.context->get_context_data(result.parms_id())->chain_index();
        int remaining_noise_budget = info.dec->invariant_noise_budget(result);
        int noise_upper_bound = init_noise_budget - remaining_noise_budget;
        std::cout<<"result : "<< level << ", " << remaining_noise_budget << ", " << noise_upper_bound << '\n';
        if(remaining_noise_budget<=0){
           success = false ;
        } 
        return success ;
    }
};