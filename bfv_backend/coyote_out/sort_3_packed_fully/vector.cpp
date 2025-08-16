
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "010000101", info);
    add_bitstring(bits, "000100000", info);
    add_bitstring(bits, "001000000", info);
    add_bitstring(bits, "000000001", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(7);
    ts[0] = encrypt_input("111111111111111111111111101111111111111111111111110111111111111111111111111101111111111111111111111111011111111111111111111111110", info);
    ts[1] = encrypt_input("111111111111111111111111111111111111111111111111110111111111111111111111111001111111111111111111111111111111111111111111111111101111111111111111111111111", info);
    ts[2] = encrypt_input("011000101", info);
    ts[4] = encrypt_input("000100000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[13];
    ctxt ss[5];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -1, gk, ss[0]); // __s0 = __v0 >> 1
    vs[1] = ts[1];
    info.eval->multiply(vs[0], vs[1], vs[2]); // __v2 = __v0 * __v1
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -1, gk, ss[1]); // __s1 = __v2 >> 1
    
    // __t3 = blend(__s0@010000101, __v0@001000000)
    {
    ctxt t3_1, t3_2;
    info.eval->multiply_plain(ss[0], bits["010000101"], t3_1);
    info.eval->multiply_plain(vs[0], bits["001000000"], t3_2);
    info.eval->add(t3_1, t3_2, ts[3]);
    }
    
    info.eval->add(ts[2], ts[3], vs[3]); // __v3 = __t2 + __t3
    info.eval->rotate_rows(vs[3], -1, gk, ss[2]); // __s2 = __v3 >> 1
    info.eval->multiply(vs[3], vs[1], vs[4]); // __v4 = __v3 * __v1
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->add(ss[1], vs[4], vs[5]); // __v5 = __s1 + __v4
    info.eval->rotate_rows(vs[5], -2, gk, ss[3]); // __s3 = __v5 >> 2
    info.eval->add(ts[4], vs[0], vs[6]); // __v6 = __t4 + __v0
    
    // __t5 = blend(__v6@000100000, __v3@000000001)
    {
    ctxt t5_1, t5_2;
    info.eval->multiply_plain(vs[6], bits["000100000"], t5_1);
    info.eval->multiply_plain(vs[3], bits["000000001"], t5_2);
    info.eval->add(t5_1, t5_2, ts[5]);
    }
    
    info.eval->multiply(ts[5], ss[3], vs[7]); // __v7 = __t5 * __s3
    info.eval->relinearize_inplace(vs[7], rk);
    info.eval->add(vs[2], vs[7], vs[8]); // __v8 = __v2 + __v7
    
    // __t6 = blend(__v8@000100000, __v1@000000001)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(vs[8], bits["000100000"], t6_1);
    info.eval->multiply_plain(vs[1], bits["000000001"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    info.eval->multiply(ss[0], ts[6], vs[9]); // __v9 = __s0 * __t6
    info.eval->relinearize_inplace(vs[9], rk);
    info.eval->add(vs[9], vs[7], vs[10]); // __v10 = __v9 + __v7
    info.eval->rotate_rows(vs[10], -4, gk, ss[4]); // __s4 = __v10 >> 4
    info.eval->multiply(ss[2], ss[4], vs[11]); // __v11 = __s2 * __s4
    info.eval->relinearize_inplace(vs[11], rk);
    info.eval->add(vs[9], vs[11], vs[12]); // __v12 = __v9 + __v11
    return vs[12];
}
    