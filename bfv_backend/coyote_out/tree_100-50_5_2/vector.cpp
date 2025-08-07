
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "0000010000000000", info);
    add_bitstring(bits, "0001000000000000", info);
    add_bitstring(bits, "0000100000000000", info);
    add_bitstring(bits, "0010000000000000", info);
    add_bitstring(bits, "0000001000000000", info);
    add_bitstring(bits, "0100000000000000", info);
    add_bitstring(bits, "0000000100000000", info);
    add_bitstring(bits, "1000000000000000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(14);
    ts[0] = encrypt_input("011111011111000000111011111100011111", info);
    ts[1] = encrypt_input("011110011111000000111011111100011111", info);
    ts[2] = encrypt_input("1111101110100111111111111111111110001111111111111110", info);
    ts[3] = encrypt_input("1111001110100111111111011110111110001111111111111110", info);
    ts[4] = encrypt_input("011111001111100000000000", info);
    ts[5] = encrypt_input("011111001111100000000000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[10];
    ctxt ss[7];

    info.eval->add(ts[0], ts[1], vs[0]); // __v0 = __t0 + __t1
    info.eval->rotate_rows(vs[0], -8, gk, ss[0]); // __s0 = __v0 >> 8
    info.eval->multiply(ts[2], ts[3], vs[1]); // __v1 = __t2 * __t3
    info.eval->relinearize_inplace(vs[1], rk);
    info.eval->rotate_rows(vs[1], -8, gk, ss[1]); // __s1 = __v1 >> 8
    
    // __t6 = blend(__s1@1000000000000000, __v0@0001000000000000, __v1@0000010000000000, __t4@0100100000000000)
    {
    ctxt t6_1, t6_2, t6_3;
    info.eval->multiply_plain(ss[1], bits["1000000000000000"], t6_1);
    info.eval->multiply_plain(vs[0], bits["0001000000000000"], t6_2);
    info.eval->multiply_plain(vs[1], bits["0000010000000000"], t6_3);
    info.eval->add_many({t6_1, t6_2, t6_3, ts[4]}, ts[6]);
    }
    
    
    // __t7 = blend(__v1@1000000000000000, __s0@0001000000000000, __s1@0000010000000000, __t5@0100100000000000)
    {
    ctxt t7_1, t7_2, t7_3;
    info.eval->multiply_plain(vs[1], bits["1000000000000000"], t7_1);
    info.eval->multiply_plain(ss[0], bits["0001000000000000"], t7_2);
    info.eval->multiply_plain(ss[1], bits["0000010000000000"], t7_3);
    info.eval->add_many({t7_1, t7_2, t7_3, ts[5]}, ts[7]);
    }
    
    info.eval->add(ts[6], ts[7], vs[2]); // __v2 = __t6 + __t7
    info.eval->rotate_rows(vs[2], -15, gk, ss[2]); // __s2 = __v2 >> 15
    
    // __t8 = blend(__v0@0100000000000000, __v1@0000001000000000, __s0@0000000100000000)
    {
    ctxt t8_1, t8_2, t8_3;
    info.eval->multiply_plain(vs[0], bits["0100000000000000"], t8_1);
    info.eval->multiply_plain(vs[1], bits["0000001000000000"], t8_2);
    info.eval->multiply_plain(ss[0], bits["0000000100000000"], t8_3);
    info.eval->add_many({t8_1, t8_2, t8_3}, ts[8]);
    }
    
    
    // __t9 = blend(__v2@0100000000000000, __s1@0000001000000000, __v1@0000000100000000)
    {
    ctxt t9_1, t9_2, t9_3;
    info.eval->multiply_plain(vs[2], bits["0100000000000000"], t9_1);
    info.eval->multiply_plain(ss[1], bits["0000001000000000"], t9_2);
    info.eval->multiply_plain(vs[1], bits["0000000100000000"], t9_3);
    info.eval->add_many({t9_1, t9_2, t9_3}, ts[9]);
    }
    
    info.eval->multiply(ts[8], ts[9], vs[3]); // __v3 = __t8 * __t9
    info.eval->relinearize_inplace(vs[3], rk);
    info.eval->rotate_rows(vs[3], -15, gk, ss[3]); // __s3 = __v3 >> 15
    info.eval->add(ss[1], vs[2], vs[4]); // __v4 = __s1 + __v2
    
    // __t10 = blend(__s3@1000000000000000, __v1@0010000000000000, __v4@0000100000000000)
    {
    ctxt t10_1, t10_2, t10_3;
    info.eval->multiply_plain(ss[3], bits["1000000000000000"], t10_1);
    info.eval->multiply_plain(vs[1], bits["0010000000000000"], t10_2);
    info.eval->multiply_plain(vs[4], bits["0000100000000000"], t10_3);
    info.eval->add_many({t10_1, t10_2, t10_3}, ts[10]);
    }
    
    
    // __t11 = blend(__v2@1000000000000000, __s0@0010000000000000, __s2@0000100000000000)
    {
    ctxt t11_1, t11_2, t11_3;
    info.eval->multiply_plain(vs[2], bits["1000000000000000"], t11_1);
    info.eval->multiply_plain(ss[0], bits["0010000000000000"], t11_2);
    info.eval->multiply_plain(ss[2], bits["0000100000000000"], t11_3);
    info.eval->add_many({t11_1, t11_2, t11_3}, ts[11]);
    }
    
    info.eval->add(ts[10], ts[11], vs[5]); // __v5 = __t10 + __t11
    info.eval->rotate_rows(vs[5], -12, gk, ss[4]); // __s4 = __v5 >> 12
    
    // __t12 = blend(__v5@0010000000000000, __v3@0000001000000000)
    {
    ctxt t12_1, t12_2;
    info.eval->multiply_plain(vs[5], bits["0010000000000000"], t12_1);
    info.eval->multiply_plain(vs[3], bits["0000001000000000"], t12_2);
    info.eval->add(t12_1, t12_2, ts[12]);
    }
    
    
    // __t13 = blend(__s2@0010000000000000, __s3@0000001000000000)
    {
    ctxt t13_1, t13_2;
    info.eval->multiply_plain(ss[2], bits["0010000000000000"], t13_1);
    info.eval->multiply_plain(ss[3], bits["0000001000000000"], t13_2);
    info.eval->add(t13_1, t13_2, ts[13]);
    }
    
    info.eval->multiply(ts[12], ts[13], vs[6]); // __v6 = __t12 * __t13
    info.eval->relinearize_inplace(vs[6], rk);
    info.eval->rotate_rows(vs[6], -12, gk, ss[5]); // __s5 = __v6 >> 12
    info.eval->multiply(ss[4], vs[5], vs[7]); // __v7 = __s4 * __v5
    info.eval->relinearize_inplace(vs[7], rk);
    info.eval->rotate_rows(vs[7], -2, gk, ss[6]); // __s6 = __v7 >> 2
    info.eval->add(vs[6], ss[5], vs[8]); // __v8 = __v6 + __s5
    info.eval->add(vs[8], ss[6], vs[9]); // __v9 = __v8 + __s6
    return vs[9];
}
    