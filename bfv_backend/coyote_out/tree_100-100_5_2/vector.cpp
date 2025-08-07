
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "000010100000", info);
    add_bitstring(bits, "000000001000", info);
    add_bitstring(bits, "100001000100", info);
    add_bitstring(bits, "100001001000", info);
    add_bitstring(bits, "100001000000", info);
    add_bitstring(bits, "000100000000", info);
    add_bitstring(bits, "010100000000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(10);
    ts[0] = encrypt_input("111011111111101111111111111111111111111111111111111111111111", info);
    ts[1] = encrypt_input("111011111111101111111111111110111101111111111111111111011110", info);
    ts[2] = encrypt_input("0111110111111111101111100000", info);
    ts[3] = encrypt_input("0111110111111111101111100000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[6];
    ctxt ss[5];

    info.eval->multiply(ts[0], ts[1], vs[0]); // __v0 = __t0 * __t1
    info.eval->relinearize_inplace(vs[0], rk);
    info.eval->rotate_rows(vs[0], -10, gk, ss[0]); // __s0 = __v0 >> 10
    
    // __t4 = blend(__v0@100001000100, __s0@000000001000, __t2@010110100000)
    {
    ctxt t4_1, t4_2;
    info.eval->multiply_plain(vs[0], bits["100001000100"], t4_1);
    info.eval->multiply_plain(ss[0], bits["000000001000"], t4_2);
    info.eval->add_many({t4_1, t4_2, ts[2]}, ts[4]);
    }
    
    
    // __t5 = blend(__s0@100001000100, __v0@000000001000, __t3@010110100000)
    {
    ctxt t5_1, t5_2;
    info.eval->multiply_plain(ss[0], bits["100001000100"], t5_1);
    info.eval->multiply_plain(vs[0], bits["000000001000"], t5_2);
    info.eval->add_many({t5_1, t5_2, ts[3]}, ts[5]);
    }
    
    info.eval->multiply(ts[4], ts[5], vs[1]); // __v1 = __t4 * __t5
    info.eval->relinearize_inplace(vs[1], rk);
    info.eval->rotate_rows(vs[1], -11, gk, ss[1]); // __s1 = __v1 >> 11
    
    // __t6 = blend(__v0@010100000000, __v1@000010100000)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(vs[0], bits["010100000000"], t6_1);
    info.eval->multiply_plain(vs[1], bits["000010100000"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    
    // __t7 = blend(__v1@010100000000, __v0@000010100000)
    {
    ctxt t7_1, t7_2;
    info.eval->multiply_plain(vs[1], bits["010100000000"], t7_1);
    info.eval->multiply_plain(vs[0], bits["000010100000"], t7_2);
    info.eval->add(t7_1, t7_2, ts[7]);
    }
    
    info.eval->multiply(ts[6], ts[7], vs[2]); // __v2 = __t6 * __t7
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -11, gk, ss[2]); // __s2 = __v2 >> 11
    
    // __t8 = blend(__v1@100001001000, __s2@000100000000)
    {
    ctxt t8_1, t8_2;
    info.eval->multiply_plain(vs[1], bits["100001001000"], t8_1);
    info.eval->multiply_plain(ss[2], bits["000100000000"], t8_2);
    info.eval->add(t8_1, t8_2, ts[8]);
    }
    
    
    // __t9 = blend(__s2@100001000000, __v2@000100000000, __s1@000000001000)
    {
    ctxt t9_1, t9_2, t9_3;
    info.eval->multiply_plain(ss[2], bits["100001000000"], t9_1);
    info.eval->multiply_plain(vs[2], bits["000100000000"], t9_2);
    info.eval->multiply_plain(ss[1], bits["000000001000"], t9_3);
    info.eval->add_many({t9_1, t9_2, t9_3}, ts[9]);
    }
    
    info.eval->multiply(ts[8], ts[9], vs[3]); // __v3 = __t8 * __t9
    info.eval->relinearize_inplace(vs[3], rk);
    info.eval->rotate_rows(vs[3], -7, gk, ss[3]); // __s3 = __v3 >> 7
    info.eval->add(vs[3], ss[3], vs[4]); // __v4 = __v3 + __s3
    info.eval->rotate_rows(vs[4], -9, gk, ss[4]); // __s4 = __v4 >> 9
    info.eval->multiply(vs[4], ss[4], vs[5]); // __v5 = __v4 * __s4
    info.eval->relinearize_inplace(vs[5], rk);
    return vs[5];
}
    