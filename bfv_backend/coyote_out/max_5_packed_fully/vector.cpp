
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "0000000000000010000000000000", info);
    add_bitstring(bits, "0001000000000000000000010000", info);
    add_bitstring(bits, "0000000000000000001000000000", info);
    add_bitstring(bits, "0010000000001000000000000100", info);
    add_bitstring(bits, "0100000000000000000000010000", info);
    add_bitstring(bits, "0000000000010000000000001000", info);
    add_bitstring(bits, "0000100000000000000000000000", info);
    add_bitstring(bits, "0000000010001000010001000000", info);
    add_bitstring(bits, "0000000001001000000000000000", info);
    add_bitstring(bits, "0001000000000000001100000000", info);
    add_bitstring(bits, "0000000001000000000000000000", info);
    add_bitstring(bits, "0000000001000100001000000000", info);
    add_bitstring(bits, "0000000000000000000000000010", info);
    add_bitstring(bits, "0000000000000001000000000000", info);
    add_bitstring(bits, "0000100000100000000000000000", info);
    add_bitstring(bits, "0000000100001000000000000000", info);
    add_bitstring(bits, "0000000010000000010001000000", info);
    add_bitstring(bits, "0000000101000000001000000000", info);
    add_bitstring(bits, "0000000000000000000000010000", info);
    add_bitstring(bits, "0000000010000000000000000000", info);
    add_bitstring(bits, "0000000001000110000000000001", info);
    add_bitstring(bits, "0000011000000001000000000010", info);
    add_bitstring(bits, "0100000000000000000000000000", info);
    add_bitstring(bits, "0000000000000000000001100000", info);
    add_bitstring(bits, "0000100000000000000000000001", info);
    add_bitstring(bits, "0001000000000000000000000000", info);
    add_bitstring(bits, "0001000000000000000100000000", info);
    add_bitstring(bits, "0000001000100000000000000000", info);
    add_bitstring(bits, "0000100000101000000000000000", info);
    add_bitstring(bits, "0000000000100000100000000000", info);
    add_bitstring(bits, "0000000001000000000000010000", info);
    add_bitstring(bits, "0000000000001000000000000000", info);
    add_bitstring(bits, "0100100000000000000000000000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(21);
    ts[0] = encrypt_input("1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110000111111111111111111111111111111111111111111111111011111111111111111111111011111111111111111111111111111111111111111111111101111111111111111111111110011111111111111111111111111111111111111111111111100111111111111111111111111000111111111111111111111111", info);
    ts[1] = encrypt_input("011111111111111111111111111111111111111111111111101111111111111111111111110011111111111111111111111101111111111111111111111110001111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111001111111111111111111111110111111111111111111111111", info);
    ts[2] = encrypt_input("0010011010111001100001101110", info);
    ts[12] = encrypt_input("0100000000000000000000000000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[17];
    ctxt ss[27];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -3, gk, ss[0]); // __s0 = __v0 >> 3
    info.eval->rotate_rows(vs[0], -11, gk, ss[1]); // __s1 = __v0 >> 11
    info.eval->rotate_rows(vs[0], -27, gk, ss[2]); // __s2 = __v0 >> 27
    info.eval->rotate_rows(vs[0], -26, gk, ss[3]); // __s3 = __v0 >> 26
    info.eval->rotate_rows(vs[0], -2, gk, ss[4]); // __s4 = __v0 >> 2
    vs[1] = ts[1];
    info.eval->rotate_rows(vs[1], -1, gk, ss[5]); // __s5 = __v1 >> 1
    info.eval->rotate_rows(vs[1], -13, gk, ss[6]); // __s6 = __v1 >> 13
    info.eval->rotate_rows(vs[1], -18, gk, ss[7]); // __s7 = __v1 >> 18
    
    // __t3 = blend(__s3@0010000000001000000000000100, __s0@0000011000000001000000000010, __s2@0000000010000000000000000000, __v0@0000000000100000100000000000, __s1@0000000000010000000000001000, __s4@0000000000000000000001100000)
    {
    ctxt t3_1, t3_2, t3_3, t3_4, t3_5, t3_6;
    info.eval->multiply_plain(ss[3], bits["0010000000001000000000000100"], t3_1);
    info.eval->multiply_plain(ss[0], bits["0000011000000001000000000010"], t3_2);
    info.eval->multiply_plain(ss[2], bits["0000000010000000000000000000"], t3_3);
    info.eval->multiply_plain(vs[0], bits["0000000000100000100000000000"], t3_4);
    info.eval->multiply_plain(ss[1], bits["0000000000010000000000001000"], t3_5);
    info.eval->multiply_plain(ss[4], bits["0000000000000000000001100000"], t3_6);
    info.eval->add_many({t3_1, t3_2, t3_3, t3_4, t3_5, t3_6}, ts[3]);
    }
    
    info.eval->add(ts[2], ts[3], vs[2]); // __v2 = __t2 + __t3
    info.eval->rotate_rows(vs[2], -27, gk, ss[8]); // __s8 = __v2 >> 27
    info.eval->rotate_rows(vs[2], -24, gk, ss[9]); // __s9 = __v2 >> 24
    info.eval->rotate_rows(vs[2], -1, gk, ss[10]); // __s10 = __v2 >> 1
    info.eval->rotate_rows(vs[2], -21, gk, ss[11]); // __s11 = __v2 >> 21
    
    // __t4 = blend(__s0@0001000000000000000100000000, __v0@0000100000000000000000000001, __s2@0000000001000100001000000000, __s1@0000000000000010000000000000)
    {
    ctxt t4_1, t4_2, t4_3, t4_4;
    info.eval->multiply_plain(ss[0], bits["0001000000000000000100000000"], t4_1);
    info.eval->multiply_plain(vs[0], bits["0000100000000000000000000001"], t4_2);
    info.eval->multiply_plain(ss[2], bits["0000000001000100001000000000"], t4_3);
    info.eval->multiply_plain(ss[1], bits["0000000000000010000000000000"], t4_4);
    info.eval->add_many({t4_1, t4_2, t4_3, t4_4}, ts[4]);
    }
    
    
    // __t5 = blend(__s5@0001000000000000001100000000, __s6@0000100000000000000000000000, __v1@0000000001000110000000000001)
    {
    ctxt t5_1, t5_2, t5_3;
    info.eval->multiply_plain(ss[5], bits["0001000000000000001100000000"], t5_1);
    info.eval->multiply_plain(ss[6], bits["0000100000000000000000000000"], t5_2);
    info.eval->multiply_plain(vs[1], bits["0000000001000110000000000001"], t5_3);
    info.eval->add_many({t5_1, t5_2, t5_3}, ts[5]);
    }
    
    info.eval->multiply(ts[4], ts[5], vs[3]); // __v3 = __t4 * __t5
    info.eval->relinearize_inplace(vs[3], rk);
    info.eval->rotate_rows(vs[3], -1, gk, ss[12]); // __s12 = __v3 >> 1
    info.eval->rotate_rows(vs[3], -27, gk, ss[13]); // __s13 = __v3 >> 27
    info.eval->rotate_rows(vs[3], -24, gk, ss[14]); // __s14 = __v3 >> 24
    info.eval->rotate_rows(vs[3], -21, gk, ss[15]); // __s15 = __v3 >> 21
    
    // __t6 = blend(__s8@0100000000000000000000000000, __s11@0000100000000000000000000000, __v2@0000001000100000000000000000, __s9@0000000010001000010001000000)
    {
    ctxt t6_1, t6_2, t6_3, t6_4;
    info.eval->multiply_plain(ss[8], bits["0100000000000000000000000000"], t6_1);
    info.eval->multiply_plain(ss[11], bits["0000100000000000000000000000"], t6_2);
    info.eval->multiply_plain(vs[2], bits["0000001000100000000000000000"], t6_3);
    info.eval->multiply_plain(ss[9], bits["0000000010001000010001000000"], t6_4);
    info.eval->add_many({t6_1, t6_2, t6_3, t6_4}, ts[6]);
    }
    
    
    // __t7 = blend(__v1@0100100000000000000000000000, __s6@0000001000100000000000000000, __s5@0000000010000000010001000000, __s7@0000000000001000000000000000)
    {
    ctxt t7_1, t7_2, t7_3, t7_4;
    info.eval->multiply_plain(vs[1], bits["0100100000000000000000000000"], t7_1);
    info.eval->multiply_plain(ss[6], bits["0000001000100000000000000000"], t7_2);
    info.eval->multiply_plain(ss[5], bits["0000000010000000010001000000"], t7_3);
    info.eval->multiply_plain(ss[7], bits["0000000000001000000000000000"], t7_4);
    info.eval->add_many({t7_1, t7_2, t7_3, t7_4}, ts[7]);
    }
    
    info.eval->multiply(ts[6], ts[7], vs[4]); // __v4 = __t6 * __t7
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->rotate_rows(vs[4], -2, gk, ss[16]); // __s16 = __v4 >> 2
    info.eval->rotate_rows(vs[4], -1, gk, ss[17]); // __s17 = __v4 >> 1
    
    // __t8 = blend(__s13@0001000000000000000000000000, __s12@0000100000100000000000000000, __s15@0000000100001000000000000000, __s14@0000000001000000000000010000, __v3@0000000000000000001000000000)
    {
    ctxt t8_1, t8_2, t8_3, t8_4, t8_5;
    info.eval->multiply_plain(ss[13], bits["0001000000000000000000000000"], t8_1);
    info.eval->multiply_plain(ss[12], bits["0000100000100000000000000000"], t8_2);
    info.eval->multiply_plain(ss[15], bits["0000000100001000000000000000"], t8_3);
    info.eval->multiply_plain(ss[14], bits["0000000001000000000000010000"], t8_4);
    info.eval->multiply_plain(vs[3], bits["0000000000000000001000000000"], t8_5);
    info.eval->add_many({t8_1, t8_2, t8_3, t8_4, t8_5}, ts[8]);
    }
    
    
    // __t9 = blend(__s16@0001000000000000000000010000, __v4@0000100000101000000000000000, __s17@0000000101000000001000000000)
    {
    ctxt t9_1, t9_2, t9_3;
    info.eval->multiply_plain(ss[16], bits["0001000000000000000000010000"], t9_1);
    info.eval->multiply_plain(vs[4], bits["0000100000101000000000000000"], t9_2);
    info.eval->multiply_plain(ss[17], bits["0000000101000000001000000000"], t9_3);
    info.eval->add_many({t9_1, t9_2, t9_3}, ts[9]);
    }
    
    info.eval->add(ts[8], ts[9], vs[5]); // __v5 = __t8 + __t9
    info.eval->rotate_rows(vs[5], -23, gk, ss[18]); // __s18 = __v5 >> 23
    info.eval->rotate_rows(vs[5], -5, gk, ss[19]); // __s19 = __v5 >> 5
    info.eval->rotate_rows(vs[5], -22, gk, ss[20]); // __s20 = __v5 >> 22
    info.eval->rotate_rows(vs[5], -16, gk, ss[21]); // __s21 = __v5 >> 16
    
    // __t10 = blend(__s10@0000000001000000000000000000, __s1@0000000000001000000000000000, __s0@0000000000000000000000010000, __v2@0000000000000000000000000010)
    {
    ctxt t10_1, t10_2, t10_3, t10_4;
    info.eval->multiply_plain(ss[10], bits["0000000001000000000000000000"], t10_1);
    info.eval->multiply_plain(ss[1], bits["0000000000001000000000000000"], t10_2);
    info.eval->multiply_plain(ss[0], bits["0000000000000000000000010000"], t10_3);
    info.eval->multiply_plain(vs[2], bits["0000000000000000000000000010"], t10_4);
    info.eval->add_many({t10_1, t10_2, t10_3, t10_4}, ts[10]);
    }
    
    
    // __t11 = blend(__v5@0000000001001000000000000000, __s19@0000000000000000000000010000, __s21@0000000000000000000000000010)
    {
    ctxt t11_1, t11_2, t11_3;
    info.eval->multiply_plain(vs[5], bits["0000000001001000000000000000"], t11_1);
    info.eval->multiply_plain(ss[19], bits["0000000000000000000000010000"], t11_2);
    info.eval->multiply_plain(ss[21], bits["0000000000000000000000000010"], t11_3);
    info.eval->add_many({t11_1, t11_2, t11_3}, ts[11]);
    }
    
    info.eval->multiply(ts[10], ts[11], vs[6]); // __v6 = __t10 * __t11
    info.eval->relinearize_inplace(vs[6], rk);
    info.eval->rotate_rows(vs[6], -17, gk, ss[22]); // __s22 = __v6 >> 17
    info.eval->add(ts[12], vs[0], vs[7]); // __v7 = __t12 + __v0
    
    // __t13 = blend(__v7@0100000000000000000000000000, __v0@0000000001000000000000000000, __s10@0000000000000000000000010000, __s0@0000000000000000000000000010)
    {
    ctxt t13_1, t13_2, t13_3, t13_4;
    info.eval->multiply_plain(vs[7], bits["0100000000000000000000000000"], t13_1);
    info.eval->multiply_plain(vs[0], bits["0000000001000000000000000000"], t13_2);
    info.eval->multiply_plain(ss[10], bits["0000000000000000000000010000"], t13_3);
    info.eval->multiply_plain(ss[0], bits["0000000000000000000000000010"], t13_4);
    info.eval->add_many({t13_1, t13_2, t13_3, t13_4}, ts[13]);
    }
    
    
    // __t14 = blend(__s20@0100000000000000000000000000, __s19@0000000001000000000000000000, __v5@0000000000000000000000010000, __s18@0000000000000000000000000010)
    {
    ctxt t14_1, t14_2, t14_3, t14_4;
    info.eval->multiply_plain(ss[20], bits["0100000000000000000000000000"], t14_1);
    info.eval->multiply_plain(ss[19], bits["0000000001000000000000000000"], t14_2);
    info.eval->multiply_plain(vs[5], bits["0000000000000000000000010000"], t14_3);
    info.eval->multiply_plain(ss[18], bits["0000000000000000000000000010"], t14_4);
    info.eval->add_many({t14_1, t14_2, t14_3, t14_4}, ts[14]);
    }
    
    info.eval->multiply(ts[13], ts[14], vs[8]); // __v8 = __t13 * __t14
    info.eval->relinearize_inplace(vs[8], rk);
    info.eval->rotate_rows(vs[8], -17, gk, ss[23]); // __s23 = __v8 >> 17
    
    // __t15 = blend(__s22@0100000000000000000000000000, __v8@0000000001000000000000000000, __s23@0000000000000001000000000000, __v6@0000000000000000000000010000)
    {
    ctxt t15_1, t15_2, t15_3, t15_4;
    info.eval->multiply_plain(ss[22], bits["0100000000000000000000000000"], t15_1);
    info.eval->multiply_plain(vs[8], bits["0000000001000000000000000000"], t15_2);
    info.eval->multiply_plain(ss[23], bits["0000000000000001000000000000"], t15_3);
    info.eval->multiply_plain(vs[6], bits["0000000000000000000000010000"], t15_4);
    info.eval->add_many({t15_1, t15_2, t15_3, t15_4}, ts[15]);
    }
    
    
    // __t16 = blend(__v8@0100000000000000000000010000, __v6@0000000001000000000000000000, __s22@0000000000000001000000000000)
    {
    ctxt t16_1, t16_2, t16_3;
    info.eval->multiply_plain(vs[8], bits["0100000000000000000000010000"], t16_1);
    info.eval->multiply_plain(vs[6], bits["0000000001000000000000000000"], t16_2);
    info.eval->multiply_plain(ss[22], bits["0000000000000001000000000000"], t16_3);
    info.eval->add_many({t16_1, t16_2, t16_3}, ts[16]);
    }
    
    info.eval->add(ts[15], ts[16], vs[9]); // __v9 = __t15 + __t16
    info.eval->rotate_rows(vs[9], -20, gk, ss[24]); // __s24 = __v9 >> 20
    info.eval->multiply(ss[8], vs[9], vs[10]); // __v10 = __s8 * __v9
    info.eval->relinearize_inplace(vs[10], rk);
    info.eval->rotate_rows(vs[10], -20, gk, ss[25]); // __s25 = __v10 >> 20
    
    // __t17 = blend(__s9@0100000000000000000000000000, __s4@0000000000000001000000000000)
    {
    ctxt t17_1, t17_2;
    info.eval->multiply_plain(ss[9], bits["0100000000000000000000000000"], t17_1);
    info.eval->multiply_plain(ss[4], bits["0000000000000001000000000000"], t17_2);
    info.eval->add(t17_1, t17_2, ts[17]);
    }
    
    
    // __t18 = blend(__s24@0100000000000000000000000000, __v9@0000000000000001000000000000)
    {
    ctxt t18_1, t18_2;
    info.eval->multiply_plain(ss[24], bits["0100000000000000000000000000"], t18_1);
    info.eval->multiply_plain(vs[9], bits["0000000000000001000000000000"], t18_2);
    info.eval->add(t18_1, t18_2, ts[18]);
    }
    
    info.eval->multiply(ts[17], ts[18], vs[11]); // __v11 = __t17 * __t18
    info.eval->relinearize_inplace(vs[11], rk);
    info.eval->multiply(ss[2], vs[9], vs[12]); // __v12 = __s2 * __v9
    info.eval->relinearize_inplace(vs[12], rk);
    
    // __t19 = blend(__v12@0100000000000000000000000000, __v11@0000000000000001000000000000)
    {
    ctxt t19_1, t19_2;
    info.eval->multiply_plain(vs[12], bits["0100000000000000000000000000"], t19_1);
    info.eval->multiply_plain(vs[11], bits["0000000000000001000000000000"], t19_2);
    info.eval->add(t19_1, t19_2, ts[19]);
    }
    
    
    // __t20 = blend(__v11@0100000000000000000000000000, __s25@0000000000000001000000000000)
    {
    ctxt t20_1, t20_2;
    info.eval->multiply_plain(vs[11], bits["0100000000000000000000000000"], t20_1);
    info.eval->multiply_plain(ss[25], bits["0000000000000001000000000000"], t20_2);
    info.eval->add(t20_1, t20_2, ts[20]);
    }
    
    info.eval->add(ts[19], ts[20], vs[13]); // __v13 = __t19 + __t20
    info.eval->rotate_rows(vs[13], -14, gk, ss[26]); // __s26 = __v13 >> 14
    info.eval->multiply(vs[2], vs[13], vs[14]); // __v14 = __v2 * __v13
    info.eval->relinearize_inplace(vs[14], rk);
    info.eval->multiply(ss[0], ss[26], vs[15]); // __v15 = __s0 * __s26
    info.eval->relinearize_inplace(vs[15], rk);
    info.eval->add(vs[15], vs[14], vs[16]); // __v16 = __v15 + __v14
    return vs[16];
}
    