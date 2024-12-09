
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "001", info);
    add_bitstring(bits, "100", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(12);
    ts[0] = encrypt_input("0011111111110", info);
    ts[1] = encrypt_input("0011111111110", info);
    ts[2] = encrypt_input("0011111111110", info);
    ts[3] = encrypt_input("0011111111110", info);
    ts[4] = encrypt_input("1111111111000", info);
    ts[5] = encrypt_input("1111111111000", info);
    ts[6] = encrypt_input("1111111111000", info);
    ts[7] = encrypt_input("1111111111000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[12];
    ctxt ss[1];

    vs[0] = ts[0];
    vs[1] = ts[1];
    vs[2] = ts[2];
    vs[3] = ts[3];
    vs[4] = ts[4];
    vs[5] = ts[5];
    vs[6] = ts[6];
    vs[7] = ts[7];
    
    // __t8 = blend(__v4@100, __v0@001)
    {
    ctxt t8_1, t8_2;
    info.eval->multiply_plain(vs[4], bits["100"], t8_1);
    info.eval->multiply_plain(vs[0], bits["001"], t8_2);
    info.eval->add(t8_1, t8_2, ts[8]);
    }
    
    
    // __t9 = blend(__v5@100, __v1@001)
    {
    ctxt t9_1, t9_2;
    info.eval->multiply_plain(vs[5], bits["100"], t9_1);
    info.eval->multiply_plain(vs[1], bits["001"], t9_2);
    info.eval->add(t9_1, t9_2, ts[9]);
    }
    
    info.eval->add(ts[8], ts[9], vs[8]); // __v8 = __t8 + __t9
    
    // __t10 = blend(__v6@100, __v2@001)
    {
    ctxt t10_1, t10_2;
    info.eval->multiply_plain(vs[6], bits["100"], t10_1);
    info.eval->multiply_plain(vs[2], bits["001"], t10_2);
    info.eval->add(t10_1, t10_2, ts[10]);
    }
    
    
    // __t11 = blend(__v7@100, __v3@001)
    {
    ctxt t11_1, t11_2;
    info.eval->multiply_plain(vs[7], bits["100"], t11_1);
    info.eval->multiply_plain(vs[3], bits["001"], t11_2);
    info.eval->add(t11_1, t11_2, ts[11]);
    }
    
    info.eval->add(ts[10], ts[11], vs[9]); // __v9 = __t10 + __t11
    info.eval->multiply(vs[8], vs[9], vs[10]); // __v10 = __v8 * __v9
    info.eval->relinearize_inplace(vs[10], rk);
    info.eval->rotate_rows(vs[10], -1, gk, ss[0]); // __s0 = __v10 >> 1
    info.eval->add(ss[0], vs[10], vs[11]); // __v11 = __s0 + __v10
    return vs[11];
}
    