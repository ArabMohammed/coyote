
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "0000000000000001", info);
    add_bitstring(bits, "0100010000000000", info);
    add_bitstring(bits, "0001000000000000", info);
    add_bitstring(bits, "0000000000010000", info);
    add_bitstring(bits, "0000010000000000", info);
    add_bitstring(bits, "0001000100000000", info);
    add_bitstring(bits, "0000000001000000", info);
    add_bitstring(bits, "0000000000000100", info);
    add_bitstring(bits, "0100000000000000", info);
    add_bitstring(bits, "0000000101000000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(14);
    ts[0] = encrypt_input("111111111111111001111100011111000000", info);
    ts[1] = encrypt_input("111111111011111001111100011110000000", info);
    ts[2] = encrypt_input("0001111111111011111111111111101110111101011111011111", info);
    ts[3] = encrypt_input("0001111111111011111111101111101110111101011111011110", info);
    ts[4] = encrypt_input("00000000000000011111", info);
    ts[5] = encrypt_input("00000000000000011111", info);
    ts[6] = encrypt_input("00000000000001111100", info);
    ts[7] = encrypt_input("00000000000001111100", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[11];
    ctxt ss[6];

    info.eval->multiply(ts[0], ts[1], vs[0]); // __v0 = __t0 * __t1
    info.eval->relinearize_inplace(vs[0], rk);
    info.eval->rotate_rows(vs[0], -1, gk, ss[0]); // __s0 = __v0 >> 1
    info.eval->add(ts[2], ts[3], vs[1]); // __v1 = __t2 + __t3
    info.eval->rotate_rows(vs[1], -1, gk, ss[1]); // __s1 = __v1 >> 1
    info.eval->add(ts[4], ts[5], vs[2]); // __v2 = __t4 + __t5
    
    // __t8 = blend(__v1@0001000100000000, __v0@0000000001000000, __v2@0000000000000001, __t6@0000000000000100)
    {
    ctxt t8_1, t8_2, t8_3;
    info.eval->multiply_plain(vs[1], bits["0001000100000000"], t8_1);
    info.eval->multiply_plain(vs[0], bits["0000000001000000"], t8_2);
    info.eval->multiply_plain(vs[2], bits["0000000000000001"], t8_3);
    info.eval->add_many({t8_1, t8_2, t8_3, ts[6]}, ts[8]);
    }
    
    
    // __t9 = blend(__s0@0001000000000000, __s1@0000000101000000, __v1@0000000000000001, __t7@0000000000000100)
    {
    ctxt t9_1, t9_2, t9_3;
    info.eval->multiply_plain(ss[0], bits["0001000000000000"], t9_1);
    info.eval->multiply_plain(ss[1], bits["0000000101000000"], t9_2);
    info.eval->multiply_plain(vs[1], bits["0000000000000001"], t9_3);
    info.eval->add_many({t9_1, t9_2, t9_3, ts[7]}, ts[9]);
    }
    
    info.eval->add(ts[8], ts[9], vs[3]); // __v3 = __t8 + __t9
    info.eval->rotate_rows(vs[3], -8, gk, ss[2]); // __s2 = __v3 >> 8
    
    // __t10 = blend(__s0@0100000000000000, __s1@0000010000000000, __v1@0000000000010000, __v3@0000000000000100)
    {
    ctxt t10_1, t10_2, t10_3, t10_4;
    info.eval->multiply_plain(ss[0], bits["0100000000000000"], t10_1);
    info.eval->multiply_plain(ss[1], bits["0000010000000000"], t10_2);
    info.eval->multiply_plain(vs[1], bits["0000000000010000"], t10_3);
    info.eval->multiply_plain(vs[3], bits["0000000000000100"], t10_4);
    info.eval->add_many({t10_1, t10_2, t10_3, t10_4}, ts[10]);
    }
    
    
    // __t11 = blend(__v0@0100010000000000, __s1@0000000000010000, __v1@0000000000000100)
    {
    ctxt t11_1, t11_2, t11_3;
    info.eval->multiply_plain(vs[0], bits["0100010000000000"], t11_1);
    info.eval->multiply_plain(ss[1], bits["0000000000010000"], t11_2);
    info.eval->multiply_plain(vs[1], bits["0000000000000100"], t11_3);
    info.eval->add_many({t11_1, t11_2, t11_3}, ts[11]);
    }
    
    info.eval->multiply(ts[10], ts[11], vs[4]); // __v4 = __t10 * __t11
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->rotate_rows(vs[4], -8, gk, ss[3]); // __s3 = __v4 >> 8
    info.eval->add(ss[2], vs[3], vs[5]); // __v5 = __s2 + __v3
    
    // __t12 = blend(__v3@0000000001000000, __v4@0000000000010000)
    {
    ctxt t12_1, t12_2;
    info.eval->multiply_plain(vs[3], bits["0000000001000000"], t12_1);
    info.eval->multiply_plain(vs[4], bits["0000000000010000"], t12_2);
    info.eval->add(t12_1, t12_2, ts[12]);
    }
    
    
    // __t13 = blend(__s3@0000000001000000, __s2@0000000000010000)
    {
    ctxt t13_1, t13_2;
    info.eval->multiply_plain(ss[3], bits["0000000001000000"], t13_1);
    info.eval->multiply_plain(ss[2], bits["0000000000010000"], t13_2);
    info.eval->add(t13_1, t13_2, ts[13]);
    }
    
    info.eval->multiply(ts[12], ts[13], vs[6]); // __v6 = __t12 * __t13
    info.eval->relinearize_inplace(vs[6], rk);
    info.eval->rotate_rows(vs[6], -4, gk, ss[4]); // __s4 = __v6 >> 4
    info.eval->multiply(ss[4], vs[5], vs[7]); // __v7 = __s4 * __v5
    info.eval->relinearize_inplace(vs[7], rk);
    info.eval->add(vs[4], ss[3], vs[8]); // __v8 = __v4 + __s3
    info.eval->add(vs[8], ss[4], vs[9]); // __v9 = __v8 + __s4
    info.eval->rotate_rows(vs[9], -2, gk, ss[5]); // __s5 = __v9 >> 2
    info.eval->add(vs[7], ss[5], vs[10]); // __v10 = __v7 + __s5
    return vs[10];
}
    