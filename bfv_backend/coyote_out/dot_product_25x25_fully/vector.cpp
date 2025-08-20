
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "0010000000100000000010000", info);
    add_bitstring(bits, "0010000000000000000000000", info);
    add_bitstring(bits, "0000000000000010000000000", info);
    add_bitstring(bits, "0000000000001000000000000", info);
    add_bitstring(bits, "0000000000000000000100000", info);
    add_bitstring(bits, "0000000000000000000000010", info);
    add_bitstring(bits, "0000000000100000000000000", info);
    add_bitstring(bits, "0000000000000010100100001", info);
    add_bitstring(bits, "0000000000000000001000000", info);
    add_bitstring(bits, "0101000000000000000000000", info);
    add_bitstring(bits, "0000000000000000000010000", info);
    add_bitstring(bits, "0000000000000100000000000", info);
    add_bitstring(bits, "0000000100000000000000000", info);
    add_bitstring(bits, "0000000010000000000000000", info);
    add_bitstring(bits, "0000000000000000000000001", info);
    add_bitstring(bits, "0101000000001000000000010", info);
    add_bitstring(bits, "0010000000000100000000000", info);
    add_bitstring(bits, "0000000000000000100000000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(9);
    ts[0] = encrypt_input("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111101111111111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111", info);
    ts[1] = encrypt_input("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111101111111111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[10];
    ctxt ss[21];

    vs[0] = ts[0];
    vs[1] = ts[1];
    info.eval->multiply(vs[0], vs[1], vs[2]); // __v2 = __v0 * __v1
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -24, gk, ss[0]); // __s0 = __v2 >> 24
    info.eval->rotate_rows(vs[2], -10, gk, ss[1]); // __s1 = __v2 >> 10
    info.eval->rotate_rows(vs[2], -7, gk, ss[2]); // __s2 = __v2 >> 7
    info.eval->rotate_rows(vs[2], -13, gk, ss[3]); // __s3 = __v2 >> 13
    info.eval->rotate_rows(vs[2], -17, gk, ss[4]); // __s4 = __v2 >> 17
    info.eval->rotate_rows(vs[2], -1, gk, ss[5]); // __s5 = __v2 >> 1
    info.eval->rotate_rows(vs[2], -2, gk, ss[6]); // __s6 = __v2 >> 2
    info.eval->rotate_rows(vs[2], -21, gk, ss[7]); // __s7 = __v2 >> 21
    
    // __t2 = blend(__v2@0101000000001000000000010, __s1@0000000000000010000000000, __s5@0000000000000000100000000, __s6@0000000000000000000100000, __s0@0000000000000000000000001)
    {
    ctxt t2_1, t2_2, t2_3, t2_4, t2_5;
    info.eval->multiply_plain(vs[2], bits["0101000000001000000000010"], t2_1);
    info.eval->multiply_plain(ss[1], bits["0000000000000010000000000"], t2_2);
    info.eval->multiply_plain(ss[5], bits["0000000000000000100000000"], t2_3);
    info.eval->multiply_plain(ss[6], bits["0000000000000000000100000"], t2_4);
    info.eval->multiply_plain(ss[0], bits["0000000000000000000000001"], t2_5);
    info.eval->add_many({t2_1, t2_2, t2_3, t2_4, t2_5}, ts[2]);
    }
    
    
    // __t3 = blend(__s4@0101000000000000000000000, __s2@0000000000001000000000000, __v2@0000000000000010100100001, __s6@0000000000000000000000010)
    {
    ctxt t3_1, t3_2, t3_3, t3_4;
    info.eval->multiply_plain(ss[4], bits["0101000000000000000000000"], t3_1);
    info.eval->multiply_plain(ss[2], bits["0000000000001000000000000"], t3_2);
    info.eval->multiply_plain(vs[2], bits["0000000000000010100100001"], t3_3);
    info.eval->multiply_plain(ss[6], bits["0000000000000000000000010"], t3_4);
    info.eval->add_many({t3_1, t3_2, t3_3, t3_4}, ts[3]);
    }
    
    info.eval->add(ts[2], ts[3], vs[3]); // __v3 = __t2 + __t3
    info.eval->rotate_rows(vs[3], -1, gk, ss[8]); // __s8 = __v3 >> 1
    info.eval->rotate_rows(vs[3], -5, gk, ss[9]); // __s9 = __v3 >> 5
    info.eval->rotate_rows(vs[3], -21, gk, ss[10]); // __s10 = __v3 >> 21
    info.eval->rotate_rows(vs[3], -2, gk, ss[11]); // __s11 = __v3 >> 2
    info.eval->rotate_rows(vs[3], -22, gk, ss[12]); // __s12 = __v3 >> 22
    info.eval->rotate_rows(vs[3], -8, gk, ss[13]); // __s13 = __v3 >> 8
    info.eval->add(ss[3], vs[3], vs[4]); // __v4 = __s3 + __v3
    info.eval->rotate_rows(vs[4], -1, gk, ss[14]); // __s14 = __v4 >> 1
    
    // __t4 = blend(__s8@0010000000000100000000000, __s13@0000000100000000000000000, __s9@0000000010000000000000000, __s10@0000000000100000000000000, __s7@0000000000000000001000000, __s12@0000000000000000000010000)
    {
    ctxt t4_1, t4_2, t4_3, t4_4, t4_5, t4_6;
    info.eval->multiply_plain(ss[8], bits["0010000000000100000000000"], t4_1);
    info.eval->multiply_plain(ss[13], bits["0000000100000000000000000"], t4_2);
    info.eval->multiply_plain(ss[9], bits["0000000010000000000000000"], t4_3);
    info.eval->multiply_plain(ss[10], bits["0000000000100000000000000"], t4_4);
    info.eval->multiply_plain(ss[7], bits["0000000000000000001000000"], t4_5);
    info.eval->multiply_plain(ss[12], bits["0000000000000000000010000"], t4_6);
    info.eval->add_many({t4_1, t4_2, t4_3, t4_4, t4_5, t4_6}, ts[4]);
    }
    
    info.eval->add(vs[2], ts[4], vs[5]); // __v5 = __v2 + __t4
    info.eval->rotate_rows(vs[5], -20, gk, ss[15]); // __s15 = __v5 >> 20
    info.eval->rotate_rows(vs[5], -2, gk, ss[16]); // __s16 = __v5 >> 2
    info.eval->add(ss[11], vs[5], vs[6]); // __v6 = __s11 + __v5
    info.eval->rotate_rows(vs[6], -20, gk, ss[17]); // __s17 = __v6 >> 20
    
    // __t5 = blend(__s15@0010000000000000000000000, __s16@0000000000100000000000000, __v5@0000000000000100000000000, __s14@0000000000000000000010000)
    {
    ctxt t5_1, t5_2, t5_3, t5_4;
    info.eval->multiply_plain(ss[15], bits["0010000000000000000000000"], t5_1);
    info.eval->multiply_plain(ss[16], bits["0000000000100000000000000"], t5_2);
    info.eval->multiply_plain(vs[5], bits["0000000000000100000000000"], t5_3);
    info.eval->multiply_plain(ss[14], bits["0000000000000000000010000"], t5_4);
    info.eval->add_many({t5_1, t5_2, t5_3, t5_4}, ts[5]);
    }
    
    
    // __t6 = blend(__v5@0010000000100000000010000, __s17@0000000000000100000000000)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(vs[5], bits["0010000000100000000010000"], t6_1);
    info.eval->multiply_plain(ss[17], bits["0000000000000100000000000"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    info.eval->add(ts[5], ts[6], vs[7]); // __v7 = __t5 + __t6
    info.eval->rotate_rows(vs[7], -11, gk, ss[18]); // __s18 = __v7 >> 11
    info.eval->rotate_rows(vs[7], -15, gk, ss[19]); // __s19 = __v7 >> 15
    
    // __t7 = blend(__v7@0000000000100000000000000, __s18@0000000000000100000000000)
    {
    ctxt t7_1, t7_2;
    info.eval->multiply_plain(vs[7], bits["0000000000100000000000000"], t7_1);
    info.eval->multiply_plain(ss[18], bits["0000000000000100000000000"], t7_2);
    info.eval->add(t7_1, t7_2, ts[7]);
    }
    
    
    // __t8 = blend(__s19@0000000000100000000000000, __v7@0000000000000100000000000)
    {
    ctxt t8_1, t8_2;
    info.eval->multiply_plain(ss[19], bits["0000000000100000000000000"], t8_1);
    info.eval->multiply_plain(vs[7], bits["0000000000000100000000000"], t8_2);
    info.eval->add(t8_1, t8_2, ts[8]);
    }
    
    info.eval->add(ts[7], ts[8], vs[8]); // __v8 = __t7 + __t8
    info.eval->rotate_rows(vs[8], -22, gk, ss[20]); // __s20 = __v8 >> 22
    info.eval->add(vs[8], ss[20], vs[9]); // __v9 = __v8 + __s20
    return vs[9];
}
    