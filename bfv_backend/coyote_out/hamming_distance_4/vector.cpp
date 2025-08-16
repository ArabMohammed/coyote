
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "00010", info);
    add_bitstring(bits, "10000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(4);
    ts[0] = encrypt_input("111111111111111111111111111111111111111111111001111111111111111111111111111111111111111111111", info);
    ts[1] = encrypt_input("111111111111111111111111111111111111111111111001111111111111111111111111111111111111111111111", info);
    ts[2] = encrypt_input("11011", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[10];
    ctxt ss[2];

    vs[0] = ts[0];
    vs[1] = ts[1];
    info.eval->multiply(vs[0], vs[1], vs[2]); // __v2 = __v0 * __v1
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->add(vs[0], vs[1], vs[3]); // __v3 = __v0 + __v1
    info.eval->multiply(ts[2], vs[2], vs[4]); // __v4 = __t2 * __v2
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->sub(vs[3], vs[4], vs[5]); // __v5 = __v3 - __v4
    info.eval->add(vs[0], vs[1], vs[6]); // __v6 = __v0 + __v1
    info.eval->sub(vs[6], vs[4], vs[7]); // __v7 = __v6 - __v4
    info.eval->rotate_rows(vs[7], -4, gk, ss[0]); // __s0 = __v7 >> 4
    
    // __t3 = blend(__v7@10000, __v5@00010)
    {
    ctxt t3_1, t3_2;
    info.eval->multiply_plain(vs[7], bits["10000"], t3_1);
    info.eval->multiply_plain(vs[5], bits["00010"], t3_2);
    info.eval->add(t3_1, t3_2, ts[3]);
    }
    
    info.eval->add(ss[0], ts[3], vs[8]); // __v8 = __s0 + __t3
    info.eval->rotate_rows(vs[8], -2, gk, ss[1]); // __s1 = __v8 >> 2
    info.eval->add(vs[8], ss[1], vs[9]); // __v9 = __v8 + __s1
    return vs[9];
}
    