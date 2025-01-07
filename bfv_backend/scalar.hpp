#include "util.hpp"
#include <iostream>
#include <map>
#include <vector>
struct ScalarProgram
{

    RuntimeContext &info;

    int num_registers();
    std::vector<std::string> vars_used();
    std::vector<ctxt> computation(std::map<std::string, ctxt> locs, RuntimeContext &info);
    std::map<std::string, ctxt> locs;

    ScalarProgram(RuntimeContext &info) : info(info)
    {

    }

    void setup()
    {
        std::string alphabet = "abcdefghijklmnopqrstuvwxyz";

        
        for (std::string c : vars_used())
        {
            ptxt dest_p;
            std::vector<int64_t> vals(info.batcher->slot_count());
            vals[0] = 1;
            info.batcher->encode(vals, dest_p);
            ctxt dest_c;
            info.enc->encrypt(dest_p, dest_c);
            locs[c] = dest_c;
        }
    }

    void run()
    {
        std::vector<ctxt> outputs = computation(locs, info);
        for(const auto &result : outputs) {
            std::cout<<"Checking the integrity of result returned by scalar code \n";
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
                throw std::out_of_range("\n===>Remaining Noise budget for output is 0 !!\n");
            }
        }
        
    }
};