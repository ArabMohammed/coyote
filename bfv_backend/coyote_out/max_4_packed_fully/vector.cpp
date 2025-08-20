
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "000000100", info);
    add_bitstring(bits, "100000000", info);
    add_bitstring(bits, "000010001", info);
    add_bitstring(bits, "010100000", info);
    add_bitstring(bits, "000100000", info);
    add_bitstring(bits, "010000000", info);
    add_bitstring(bits, "000100010", info);
    add_bitstring(bits, "000001000", info);
    add_bitstring(bits, "000000010", info);
    add_bitstring(bits, "000010011", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(15);
    ts[0] = encrypt_input("11111111111111111111111011111111111111111111111111111111111111111111111111111111111111111111111101111111111111111111111111111111111111111111111111111111111111111111111110", info);
    ts[1] = encrypt_input("1111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111", info);
    ts[2] = encrypt_input("000011011", info);
    ts[5] = encrypt_input("100100000", info);
    ts[12] = encrypt_input("010000000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[15];
    ctxt ss[7];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -1, gk, ss[0]); // __s0 = __v0 >> 1
    info.eval->rotate_rows(vs[0], -8, gk, ss[1]); // __s1 = __v0 >> 8
    vs[1] = ts[1];
    
    // __t3 = blend(__s0@000010011, __v0@000001000)
    {
    ctxt t3_1, t3_2;
    info.eval->multiply_plain(ss[0], bits["000010011"], t3_1);
    info.eval->multiply_plain(vs[0], bits["000001000"], t3_2);
    info.eval->add(t3_1, t3_2, ts[3]);
    }
    
    info.eval->add(ts[2], ts[3], vs[2]); // __v2 = __t2 + __t3
    
    // __t4 = blend(__v0@010100000, __v2@000010001, __s0@000000100)
    {
    ctxt t4_1, t4_2, t4_3;
    info.eval->multiply_plain(vs[0], bits["010100000"], t4_1);
    info.eval->multiply_plain(vs[2], bits["000010001"], t4_2);
    info.eval->multiply_plain(ss[0], bits["000000100"], t4_3);
    info.eval->add_many({t4_1, t4_2, t4_3}, ts[4]);
    }
    
    info.eval->multiply(ts[4], vs[1], vs[3]); // __v3 = __t4 * __v1
    info.eval->relinearize_inplace(vs[3], rk);
    info.eval->rotate_rows(vs[3], -8, gk, ss[2]); // __s2 = __v3 >> 8
    
    // __t6 = blend(__s1@100000000, __s0@000100000)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(ss[1], bits["100000000"], t6_1);
    info.eval->multiply_plain(ss[0], bits["000100000"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    info.eval->add(ts[5], ts[6], vs[4]); // __v4 = __t5 + __t6
    
    // __t7 = blend(__v4@100000000, __v2@000001000, __v0@000000010)
    {
    ctxt t7_1, t7_2, t7_3;
    info.eval->multiply_plain(vs[4], bits["100000000"], t7_1);
    info.eval->multiply_plain(vs[2], bits["000001000"], t7_2);
    info.eval->multiply_plain(vs[0], bits["000000010"], t7_3);
    info.eval->add_many({t7_1, t7_2, t7_3}, ts[7]);
    }
    
    info.eval->multiply(ts[7], vs[1], vs[5]); // __v5 = __t7 * __v1
    info.eval->relinearize_inplace(vs[5], rk);
    
    // __t8 = blend(__s2@100000000, __v3@000100000, __v5@000000010)
    {
    ctxt t8_1, t8_2, t8_3;
    info.eval->multiply_plain(ss[2], bits["100000000"], t8_1);
    info.eval->multiply_plain(vs[3], bits["000100000"], t8_2);
    info.eval->multiply_plain(vs[5], bits["000000010"], t8_3);
    info.eval->add_many({t8_1, t8_2, t8_3}, ts[8]);
    }
    
    
    // __t9 = blend(__v5@100000000, __s2@000100010)
    {
    ctxt t9_1, t9_2;
    info.eval->multiply_plain(vs[5], bits["100000000"], t9_1);
    info.eval->multiply_plain(ss[2], bits["000100010"], t9_2);
    info.eval->add(t9_1, t9_2, ts[9]);
    }
    
    info.eval->add(ts[8], ts[9], vs[6]); // __v6 = __t8 + __t9
    info.eval->rotate_rows(vs[6], -3, gk, ss[3]); // __s3 = __v6 >> 3
    info.eval->multiply(vs[4], vs[6], vs[7]); // __v7 = __v4 * __v6
    info.eval->relinearize_inplace(vs[7], rk);
    info.eval->add(ss[2], vs[5], vs[8]); // __v8 = __s2 + __v5
    
    // __t10 = blend(__s0@000100000, __s1@000001000, __v2@000000010)
    {
    ctxt t10_1, t10_2, t10_3;
    info.eval->multiply_plain(ss[0], bits["000100000"], t10_1);
    info.eval->multiply_plain(ss[1], bits["000001000"], t10_2);
    info.eval->multiply_plain(vs[2], bits["000000010"], t10_3);
    info.eval->add_many({t10_1, t10_2, t10_3}, ts[10]);
    }
    
    
    // __t11 = blend(__s3@000100000, __v8@000001000, __v6@000000010)
    {
    ctxt t11_1, t11_2, t11_3;
    info.eval->multiply_plain(ss[3], bits["000100000"], t11_1);
    info.eval->multiply_plain(vs[8], bits["000001000"], t11_2);
    info.eval->multiply_plain(vs[6], bits["000000010"], t11_3);
    info.eval->add_many({t11_1, t11_2, t11_3}, ts[11]);
    }
    
    info.eval->multiply(ts[10], ts[11], vs[9]); // __v9 = __t10 * __t11
    info.eval->relinearize_inplace(vs[9], rk);
    info.eval->rotate_rows(vs[9], -7, gk, ss[4]); // __s4 = __v9 >> 7
    info.eval->add(vs[9], vs[7], vs[10]); // __v10 = __v9 + __v7
    info.eval->rotate_rows(vs[10], -7, gk, ss[5]); // __s5 = __v10 >> 7
    
    // __t13 = blend(__v9@000001000, __t12@010000000)
    {
    ctxt t13_1;
    info.eval->multiply_plain(vs[9], bits["000001000"], t13_1);
    info.eval->add(t13_1, ts[12], ts[13]);
    }
    
    
    // __t14 = blend(__s0@010000000, __s4@000001000)
    {
    ctxt t14_1, t14_2;
    info.eval->multiply_plain(ss[0], bits["010000000"], t14_1);
    info.eval->multiply_plain(ss[4], bits["000001000"], t14_2);
    info.eval->add(t14_1, t14_2, ts[14]);
    }
    
    info.eval->add(ts[13], ts[14], vs[11]); // __v11 = __t13 + __t14
    info.eval->rotate_rows(vs[11], -5, gk, ss[6]); // __s6 = __v11 >> 5
    info.eval->multiply(vs[11], ss[5], vs[12]); // __v12 = __v11 * __s5
    info.eval->relinearize_inplace(vs[12], rk);
    info.eval->multiply(ss[0], ss[6], vs[13]); // __v13 = __s0 * __s6
    info.eval->relinearize_inplace(vs[13], rk);
    info.eval->add(vs[13], vs[12], vs[14]); // __v14 = __v13 + __v12
    return vs[14];
}
    