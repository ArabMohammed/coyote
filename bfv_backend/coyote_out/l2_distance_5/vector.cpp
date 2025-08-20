
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "00010", info);
    add_bitstring(bits, "11101", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(4);
    ts[0] = encrypt_input("111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111", info);
    ts[1] = encrypt_input("111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[9];
    ctxt ss[3];

    vs[0] = ts[0];
    vs[1] = ts[1];
    info.eval->sub(vs[0], vs[1], vs[2]); // __v2 = __v0 - __v1
    info.eval->sub(vs[0], vs[1], vs[3]); // __v3 = __v0 - __v1
    
    // __t2 = blend(__v3@11101, __v2@00010)
    {
    ctxt t2_1, t2_2;
    info.eval->multiply_plain(vs[3], bits["11101"], t2_1);
    info.eval->multiply_plain(vs[2], bits["00010"], t2_2);
    info.eval->add(t2_1, t2_2, ts[2]);
    }
    
    
    // __t3 = blend(__v2@11101, __v3@00010)
    {
    ctxt t3_1, t3_2;
    info.eval->multiply_plain(vs[2], bits["11101"], t3_1);
    info.eval->multiply_plain(vs[3], bits["00010"], t3_2);
    info.eval->add(t3_1, t3_2, ts[3]);
    }
    
    info.eval->multiply(ts[2], ts[3], vs[4]); // __v4 = __t2 * __t3
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->rotate_rows(vs[4], -2, gk, ss[0]); // __s0 = __v4 >> 2
    info.eval->rotate_rows(vs[4], -1, gk, ss[1]); // __s1 = __v4 >> 1
    info.eval->add(vs[4], ss[1], vs[5]); // __v5 = __v4 + __s1
    info.eval->add(ss[0], vs[5], vs[6]); // __v6 = __s0 + __v5
    info.eval->rotate_rows(vs[6], -2, gk, ss[2]); // __s2 = __v6 >> 2
    info.eval->add(ss[1], vs[4], vs[7]); // __v7 = __s1 + __v4
    info.eval->add(vs[7], ss[2], vs[8]); // __v8 = __v7 + __s2
    return vs[8];
}
    