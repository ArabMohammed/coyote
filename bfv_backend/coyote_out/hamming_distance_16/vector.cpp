
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "00000000000000000000000000010000", info);
    add_bitstring(bits, "01010000001000000000000000000000", info);
    add_bitstring(bits, "00000000010000000000000000101000", info);
    add_bitstring(bits, "01000000000000000000000000001000", info);
    add_bitstring(bits, "00000000000000001010000000000000", info);
    add_bitstring(bits, "01010000000000000000000100000001", info);
    add_bitstring(bits, "00000000000010000000000100000000", info);
    add_bitstring(bits, "00000000001000011000000000000000", info);
    add_bitstring(bits, "00000010000000000000001000000000", info);
    add_bitstring(bits, "00010000010000000000010000000000", info);
    add_bitstring(bits, "00000000000100000000010000000000", info);
    add_bitstring(bits, "00000000000001000000000000000000", info);
    add_bitstring(bits, "00000001000000000000100010000000", info);
    add_bitstring(bits, "00000000000000000010000000000000", info);
    add_bitstring(bits, "00000000000000100000000000000000", info);
    add_bitstring(bits, "00000101000000000000000000000000", info);
    add_bitstring(bits, "00000000010000000000000000000000", info);
    add_bitstring(bits, "00000000000000000000000001000001", info);
    add_bitstring(bits, "00000000000000000000000000001001", info);
    add_bitstring(bits, "00000000000100000000011000000010", info);
    add_bitstring(bits, "00000000001000000011000000000000", info);
    add_bitstring(bits, "00100000000000100000010000000010", info);
    add_bitstring(bits, "00000000000010000000000000000000", info);
    add_bitstring(bits, "00000000000000000000000000000001", info);
    add_bitstring(bits, "00100000101001000101000000000100", info);
    add_bitstring(bits, "10000000000000000000000000000000", info);
    add_bitstring(bits, "00001000001000000000000000000100", info);
    add_bitstring(bits, "00000000000100000000000000000000", info);
    add_bitstring(bits, "00000000000000000000000000000100", info);
    add_bitstring(bits, "00100000000000000000100000000000", info);
    add_bitstring(bits, "00000010000010000000000100000000", info);
    add_bitstring(bits, "10000000000000000000001000000000", info);
    add_bitstring(bits, "00000000000000000101000000000000", info);
    add_bitstring(bits, "00000010000000000000010000000000", info);
    add_bitstring(bits, "00000000000000000000000100000001", info);
    add_bitstring(bits, "00010000000000000000000000000000", info);
    add_bitstring(bits, "00000000100100000100000000010000", info);
    add_bitstring(bits, "00000000010010000000100000000000", info);
    add_bitstring(bits, "00010000000010000000000000100000", info);
    add_bitstring(bits, "00000000010000000000000001000000", info);
    add_bitstring(bits, "00010000010000000000000000000000", info);
    add_bitstring(bits, "00100000100000000000000100000000", info);
    add_bitstring(bits, "00000000100000100000000000000000", info);
    add_bitstring(bits, "00000010000010000000000000001000", info);
    add_bitstring(bits, "10000010000001010000000000000000", info);
    add_bitstring(bits, "00000000000000010000100000000000", info);
    add_bitstring(bits, "00001000000000001010000000000000", info);
    add_bitstring(bits, "00000000000000000000110010000000", info);
    add_bitstring(bits, "10000000000000000000011000000000", info);
    add_bitstring(bits, "00000000000000000000000010000000", info);
    add_bitstring(bits, "00010000000000000000010000000000", info);
    add_bitstring(bits, "10000000000000000001000000000000", info);
    add_bitstring(bits, "00000000010000000010000000000010", info);
    add_bitstring(bits, "00000010000001000100000000000100", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(15);
    ts[0] = encrypt_input("0011111111111111111111111101111111111111111111111111001111111111111111111111110111111111111111111111111101111111111111111111111111111111111111111111111111011111111111111111111111101111111111111111111111110111111111111111111111111111111111111111111111111110111111111111111111111111001111111111111111111111100111111111111111111111111111111111111111111111111011111111111111111111111101111111111111111111111110", info);
    ts[1] = encrypt_input("1111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111101111111111111111111111110001111111111111111111111110111111111111111111111111101111111111111111111111111111111111111111111111110111111111111111111111110000001111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111000", info);
    ts[6] = encrypt_input("01010001011110111010100110000011", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[10];
    ctxt ss[42];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -31, gk, ss[0]); // __s0 = __v0 >> 31
    info.eval->rotate_rows(vs[0], -20, gk, ss[1]); // __s1 = __v0 >> 20
    info.eval->rotate_rows(vs[0], -25, gk, ss[2]); // __s2 = __v0 >> 25
    info.eval->rotate_rows(vs[0], -29, gk, ss[3]); // __s3 = __v0 >> 29
    info.eval->rotate_rows(vs[0], -27, gk, ss[4]); // __s4 = __v0 >> 27
    info.eval->rotate_rows(vs[0], -28, gk, ss[5]); // __s5 = __v0 >> 28
    info.eval->rotate_rows(vs[0], -11, gk, ss[6]); // __s6 = __v0 >> 11
    info.eval->rotate_rows(vs[0], -1, gk, ss[7]); // __s7 = __v0 >> 1
    vs[1] = ts[1];
    info.eval->rotate_rows(vs[1], -4, gk, ss[8]); // __s8 = __v1 >> 4
    info.eval->rotate_rows(vs[1], -1, gk, ss[9]); // __s9 = __v1 >> 1
    info.eval->rotate_rows(vs[1], -5, gk, ss[10]); // __s10 = __v1 >> 5
    info.eval->rotate_rows(vs[1], -6, gk, ss[11]); // __s11 = __v1 >> 6
    info.eval->rotate_rows(vs[1], -20, gk, ss[12]); // __s12 = __v1 >> 20
    info.eval->rotate_rows(vs[1], -31, gk, ss[13]); // __s13 = __v1 >> 31
    info.eval->rotate_rows(vs[1], -30, gk, ss[14]); // __s14 = __v1 >> 30
    info.eval->rotate_rows(vs[1], -24, gk, ss[15]); // __s15 = __v1 >> 24
    
    // __t2 = blend(__s0@01010000001000000000000000000000, __s3@00001000000000001010000000000000, __s2@00000101000000000000000000000000, __v0@00000000010000000000000000101000, __s5@00000000000010000000000100000000, __s7@00000000000000000000000001000001, __s6@00000000000000000000000000000100)
    {
    ctxt t2_1, t2_2, t2_3, t2_4, t2_5, t2_6, t2_7;
    info.eval->multiply_plain(ss[0], bits["01010000001000000000000000000000"], t2_1);
    info.eval->multiply_plain(ss[3], bits["00001000000000001010000000000000"], t2_2);
    info.eval->multiply_plain(ss[2], bits["00000101000000000000000000000000"], t2_3);
    info.eval->multiply_plain(vs[0], bits["00000000010000000000000000101000"], t2_4);
    info.eval->multiply_plain(ss[5], bits["00000000000010000000000100000000"], t2_5);
    info.eval->multiply_plain(ss[7], bits["00000000000000000000000001000001"], t2_6);
    info.eval->multiply_plain(ss[6], bits["00000000000000000000000000000100"], t2_7);
    info.eval->add_many({t2_1, t2_2, t2_3, t2_4, t2_5, t2_6, t2_7}, ts[2]);
    }
    
    
    // __t3 = blend(__v1@01000000000000000000000000001000, __s13@00010000000010000000000000100000, __s8@00001000001000000000000000000100, __s14@00000101000000000000000000000000, __s11@00000000010000000000000001000000, __s9@00000000000000001010000000000000, __s10@00000000000000000000000100000001)
    {
    ctxt t3_1, t3_2, t3_3, t3_4, t3_5, t3_6, t3_7;
    info.eval->multiply_plain(vs[1], bits["01000000000000000000000000001000"], t3_1);
    info.eval->multiply_plain(ss[13], bits["00010000000010000000000000100000"], t3_2);
    info.eval->multiply_plain(ss[8], bits["00001000001000000000000000000100"], t3_3);
    info.eval->multiply_plain(ss[14], bits["00000101000000000000000000000000"], t3_4);
    info.eval->multiply_plain(ss[11], bits["00000000010000000000000001000000"], t3_5);
    info.eval->multiply_plain(ss[9], bits["00000000000000001010000000000000"], t3_6);
    info.eval->multiply_plain(ss[10], bits["00000000000000000000000100000001"], t3_7);
    info.eval->add_many({t3_1, t3_2, t3_3, t3_4, t3_5, t3_6, t3_7}, ts[3]);
    }
    
    info.eval->multiply(ts[2], ts[3], vs[2]); // __v2 = __t2 * __t3
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -6, gk, ss[16]); // __s16 = __v2 >> 6
    info.eval->rotate_rows(vs[2], -9, gk, ss[17]); // __s17 = __v2 >> 9
    info.eval->rotate_rows(vs[2], -2, gk, ss[18]); // __s18 = __v2 >> 2
    info.eval->rotate_rows(vs[2], -27, gk, ss[19]); // __s19 = __v2 >> 27
    info.eval->rotate_rows(vs[2], -25, gk, ss[20]); // __s20 = __v2 >> 25
    info.eval->rotate_rows(vs[2], -18, gk, ss[21]); // __s21 = __v2 >> 18
    
    // __t4 = blend(__s2@10000000000000000001000000000000, __v0@00100000000000100000010000000010, __s4@00000010000000000000001000000000, __s0@00000000100100000100000000010000, __s3@00000000000001000000000000000000, __s5@00000000000000010000100000000000, __s1@00000000000000000000000010000000)
    {
    ctxt t4_1, t4_2, t4_3, t4_4, t4_5, t4_6, t4_7;
    info.eval->multiply_plain(ss[2], bits["10000000000000000001000000000000"], t4_1);
    info.eval->multiply_plain(vs[0], bits["00100000000000100000010000000010"], t4_2);
    info.eval->multiply_plain(ss[4], bits["00000010000000000000001000000000"], t4_3);
    info.eval->multiply_plain(ss[0], bits["00000000100100000100000000010000"], t4_4);
    info.eval->multiply_plain(ss[3], bits["00000000000001000000000000000000"], t4_5);
    info.eval->multiply_plain(ss[5], bits["00000000000000010000100000000000"], t4_6);
    info.eval->multiply_plain(ss[1], bits["00000000000000000000000010000000"], t4_7);
    info.eval->add_many({t4_1, t4_2, t4_3, t4_4, t4_5, t4_6, t4_7}, ts[4]);
    }
    
    
    // __t5 = blend(__v1@10000010000001010000000000000000, __s9@00100000000000000000100000000000, __s10@00000000100000100000000000000000, __s8@00000000000100000000011000000010, __s15@00000000000000000101000000000000, __s12@00000000000000000000000010000000, __s13@00000000000000000000000000010000)
    {
    ctxt t5_1, t5_2, t5_3, t5_4, t5_5, t5_6, t5_7;
    info.eval->multiply_plain(vs[1], bits["10000010000001010000000000000000"], t5_1);
    info.eval->multiply_plain(ss[9], bits["00100000000000000000100000000000"], t5_2);
    info.eval->multiply_plain(ss[10], bits["00000000100000100000000000000000"], t5_3);
    info.eval->multiply_plain(ss[8], bits["00000000000100000000011000000010"], t5_4);
    info.eval->multiply_plain(ss[15], bits["00000000000000000101000000000000"], t5_5);
    info.eval->multiply_plain(ss[12], bits["00000000000000000000000010000000"], t5_6);
    info.eval->multiply_plain(ss[13], bits["00000000000000000000000000010000"], t5_7);
    info.eval->add_many({t5_1, t5_2, t5_3, t5_4, t5_5, t5_6, t5_7}, ts[5]);
    }
    
    info.eval->add(ts[4], ts[5], vs[3]); // __v3 = __t4 + __t5
    info.eval->rotate_rows(vs[3], -2, gk, ss[22]); // __s22 = __v3 >> 2
    info.eval->rotate_rows(vs[3], -9, gk, ss[23]); // __s23 = __v3 >> 9
    info.eval->rotate_rows(vs[3], -25, gk, ss[24]); // __s24 = __v3 >> 25
    info.eval->rotate_rows(vs[3], -27, gk, ss[25]); // __s25 = __v3 >> 27
    info.eval->rotate_rows(vs[3], -28, gk, ss[26]); // __s26 = __v3 >> 28
    
    // __t7 = blend(__v2@01010000000000000000000100000001, __s19@00000001000000000000100010000000, __s18@00000000010000000010000000000010, __s16@00000000001000011000000000000000, __s20@00000000000100000000000000000000, __s21@00000000000010000000000000000000, __s17@00000000000000100000000000000000)
    {
    ctxt t7_1, t7_2, t7_3, t7_4, t7_5, t7_6, t7_7;
    info.eval->multiply_plain(vs[2], bits["01010000000000000000000100000001"], t7_1);
    info.eval->multiply_plain(ss[19], bits["00000001000000000000100010000000"], t7_2);
    info.eval->multiply_plain(ss[18], bits["00000000010000000010000000000010"], t7_3);
    info.eval->multiply_plain(ss[16], bits["00000000001000011000000000000000"], t7_4);
    info.eval->multiply_plain(ss[20], bits["00000000000100000000000000000000"], t7_5);
    info.eval->multiply_plain(ss[21], bits["00000000000010000000000000000000"], t7_6);
    info.eval->multiply_plain(ss[17], bits["00000000000000100000000000000000"], t7_7);
    info.eval->add_many({t7_1, t7_2, t7_3, t7_4, t7_5, t7_6, t7_7}, ts[7]);
    }
    
    info.eval->multiply(ts[6], ts[7], vs[4]); // __v4 = __t6 * __t7
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->rotate_rows(vs[4], -10, gk, ss[27]); // __s27 = __v4 >> 10
    info.eval->rotate_rows(vs[4], -21, gk, ss[28]); // __s28 = __v4 >> 21
    info.eval->rotate_rows(vs[4], -31, gk, ss[29]); // __s29 = __v4 >> 31
    info.eval->rotate_rows(vs[4], -24, gk, ss[30]); // __s30 = __v4 >> 24
    info.eval->rotate_rows(vs[4], -27, gk, ss[31]); // __s31 = __v4 >> 27
    
    // __t8 = blend(__s22@00100000101001000101000000000100, __s24@00000010000010000000000100000000, __s25@00000000010000000000000000000000, __s23@00000000000100000000000000000000, __s26@00000000000000000010000000000000, __v3@00000000000000000000110010000000)
    {
    ctxt t8_1, t8_2, t8_3, t8_4, t8_5, t8_6;
    info.eval->multiply_plain(ss[22], bits["00100000101001000101000000000100"], t8_1);
    info.eval->multiply_plain(ss[24], bits["00000010000010000000000100000000"], t8_2);
    info.eval->multiply_plain(ss[25], bits["00000000010000000000000000000000"], t8_3);
    info.eval->multiply_plain(ss[23], bits["00000000000100000000000000000000"], t8_4);
    info.eval->multiply_plain(ss[26], bits["00000000000000000010000000000000"], t8_5);
    info.eval->multiply_plain(vs[3], bits["00000000000000000000110010000000"], t8_6);
    info.eval->add_many({t8_1, t8_2, t8_3, t8_4, t8_5, t8_6}, ts[8]);
    }
    
    
    // __t9 = blend(__s30@00100000100000000000000100000000, __s29@00000010000001000100000000000100, __v4@00000000010010000000100000000000, __s31@00000000001000000011000000000000, __s27@00000000000100000000010000000000, __s28@00000000000000000000000010000000)
    {
    ctxt t9_1, t9_2, t9_3, t9_4, t9_5, t9_6;
    info.eval->multiply_plain(ss[30], bits["00100000100000000000000100000000"], t9_1);
    info.eval->multiply_plain(ss[29], bits["00000010000001000100000000000100"], t9_2);
    info.eval->multiply_plain(vs[4], bits["00000000010010000000100000000000"], t9_3);
    info.eval->multiply_plain(ss[31], bits["00000000001000000011000000000000"], t9_4);
    info.eval->multiply_plain(ss[27], bits["00000000000100000000010000000000"], t9_5);
    info.eval->multiply_plain(ss[28], bits["00000000000000000000000010000000"], t9_6);
    info.eval->add_many({t9_1, t9_2, t9_3, t9_4, t9_5, t9_6}, ts[9]);
    }
    
    info.eval->sub(ts[8], ts[9], vs[5]); // __v5 = __t8 - __t9
    info.eval->rotate_rows(vs[5], -4, gk, ss[32]); // __s32 = __v5 >> 4
    info.eval->rotate_rows(vs[5], -18, gk, ss[33]); // __s33 = __v5 >> 18
    info.eval->rotate_rows(vs[5], -24, gk, ss[34]); // __s34 = __v5 >> 24
    info.eval->rotate_rows(vs[5], -23, gk, ss[35]); // __s35 = __v5 >> 23
    info.eval->rotate_rows(vs[5], -12, gk, ss[36]); // __s36 = __v5 >> 12
    
    // __t10 = blend(__s35@00010000010000000000000000000000, __v5@00000010000000000000010000000000, __s34@00000000000010000000000000000000, __s32@00000000000000000000000000010000, __s33@00000000000000000000000000001001)
    {
    ctxt t10_1, t10_2, t10_3, t10_4, t10_5;
    info.eval->multiply_plain(ss[35], bits["00010000010000000000000000000000"], t10_1);
    info.eval->multiply_plain(vs[5], bits["00000010000000000000010000000000"], t10_2);
    info.eval->multiply_plain(ss[34], bits["00000000000010000000000000000000"], t10_3);
    info.eval->multiply_plain(ss[32], bits["00000000000000000000000000010000"], t10_4);
    info.eval->multiply_plain(ss[33], bits["00000000000000000000000000001001"], t10_5);
    info.eval->add_many({t10_1, t10_2, t10_3, t10_4, t10_5}, ts[10]);
    }
    
    
    // __t11 = blend(__s34@00010000010000000000010000000000, __s32@00000010000010000000000000001000, __s33@00000000000000000000000000010000, __s36@00000000000000000000000000000001)
    {
    ctxt t11_1, t11_2, t11_3, t11_4;
    info.eval->multiply_plain(ss[34], bits["00010000010000000000010000000000"], t11_1);
    info.eval->multiply_plain(ss[32], bits["00000010000010000000000000001000"], t11_2);
    info.eval->multiply_plain(ss[33], bits["00000000000000000000000000010000"], t11_3);
    info.eval->multiply_plain(ss[36], bits["00000000000000000000000000000001"], t11_4);
    info.eval->add_many({t11_1, t11_2, t11_3, t11_4}, ts[11]);
    }
    
    info.eval->add(ts[10], ts[11], vs[6]); // __v6 = __t10 + __t11
    info.eval->rotate_rows(vs[6], -26, gk, ss[37]); // __s37 = __v6 >> 26
    info.eval->rotate_rows(vs[6], -23, gk, ss[38]); // __s38 = __v6 >> 23
    
    // __t12 = blend(__s37@10000000000000000000011000000000, __s38@00010000000000000000000000000000)
    {
    ctxt t12_1, t12_2;
    info.eval->multiply_plain(ss[37], bits["10000000000000000000011000000000"], t12_1);
    info.eval->multiply_plain(ss[38], bits["00010000000000000000000000000000"], t12_2);
    info.eval->add(t12_1, t12_2, ts[12]);
    }
    
    
    // __t13 = blend(__s38@10000000000000000000001000000000, __v6@00010000000000000000010000000000)
    {
    ctxt t13_1, t13_2;
    info.eval->multiply_plain(ss[38], bits["10000000000000000000001000000000"], t13_1);
    info.eval->multiply_plain(vs[6], bits["00010000000000000000010000000000"], t13_2);
    info.eval->add(t13_1, t13_2, ts[13]);
    }
    
    info.eval->add(ts[12], ts[13], vs[7]); // __v7 = __t12 + __t13
    info.eval->rotate_rows(vs[7], -14, gk, ss[39]); // __s39 = __v7 >> 14
    info.eval->rotate_rows(vs[7], -10, gk, ss[40]); // __s40 = __v7 >> 10
    
    // __t14 = blend(__s40@10000000000000000000000000000000, __s39@00010000000000000000000000000000)
    {
    ctxt t14_1, t14_2;
    info.eval->multiply_plain(ss[40], bits["10000000000000000000000000000000"], t14_1);
    info.eval->multiply_plain(ss[39], bits["00010000000000000000000000000000"], t14_2);
    info.eval->add(t14_1, t14_2, ts[14]);
    }
    
    info.eval->add(vs[7], ts[14], vs[8]); // __v8 = __v7 + __t14
    info.eval->rotate_rows(vs[8], -3, gk, ss[41]); // __s41 = __v8 >> 3
    info.eval->add(vs[8], ss[41], vs[9]); // __v9 = __v8 + __s41
    return vs[9];
}
    