
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "00000000010000000000000000", info);
    add_bitstring(bits, "00000000000010000000000100", info);
    add_bitstring(bits, "00000000000000000000000001", info);
    add_bitstring(bits, "00000000000000000000100000", info);
    add_bitstring(bits, "00000010000000001000010000", info);
    add_bitstring(bits, "00000010000000000000000000", info);
    add_bitstring(bits, "00000000000000000000001000", info);
    add_bitstring(bits, "00100000000000000010000000", info);
    add_bitstring(bits, "01000000010010000000000000", info);
    add_bitstring(bits, "00000000001000000000001000", info);
    add_bitstring(bits, "00000001001000000000000000", info);
    add_bitstring(bits, "00000100000000000000000100", info);
    add_bitstring(bits, "00000000110000000001010100", info);
    add_bitstring(bits, "00000000000000000000001010", info);
    add_bitstring(bits, "11011000000000100100000000", info);
    add_bitstring(bits, "10000000000000001000000000", info);
    add_bitstring(bits, "00000000000000001001010000", info);
    add_bitstring(bits, "00000000000000010000001000", info);
    add_bitstring(bits, "01000000000000000010000000", info);
    add_bitstring(bits, "00000100000000000000000000", info);
    add_bitstring(bits, "00101000000000100000000000", info);
    add_bitstring(bits, "00000000000100000010000010", info);
    add_bitstring(bits, "00000000000000001000000000", info);
    add_bitstring(bits, "00000000000000000000010000", info);
    add_bitstring(bits, "00000000000000000100000001", info);
    add_bitstring(bits, "00000000000010000000000001", info);
    add_bitstring(bits, "00000000000010100000000010", info);
    add_bitstring(bits, "00000000010000000000100010", info);
    add_bitstring(bits, "00000000000000010000010000", info);
    add_bitstring(bits, "00000010100100000000000000", info);
    add_bitstring(bits, "00000010000000001000000000", info);
    add_bitstring(bits, "00000100000101010010000000", info);
    add_bitstring(bits, "00000000001000000001000000", info);
    add_bitstring(bits, "00000000000000000000001001", info);
    add_bitstring(bits, "00000000000000000010000000", info);
    add_bitstring(bits, "00000000000001000000000000", info);
    add_bitstring(bits, "00000000100000000000000000", info);
    add_bitstring(bits, "00000000100000000000001000", info);
    add_bitstring(bits, "00001100100000000000000001", info);
    add_bitstring(bits, "00000000010000000001000000", info);
    add_bitstring(bits, "00000000000010000000000000", info);
    add_bitstring(bits, "00000000000000000000000010", info);
    add_bitstring(bits, "00000000000000000001000000", info);
    add_bitstring(bits, "00000000000000001100000000", info);
    add_bitstring(bits, "00000000010000100000000000", info);
    add_bitstring(bits, "00100000000000000000000000", info);
    add_bitstring(bits, "00000000000000000000000100", info);
    add_bitstring(bits, "00000000000000010000000000", info);
    add_bitstring(bits, "00000010000000000001000000", info);
    add_bitstring(bits, "00000110000000000000000000", info);
    add_bitstring(bits, "10001000000000101000000000", info);
    add_bitstring(bits, "00000010000100000000000000", info);
    add_bitstring(bits, "00010001000100000000000000", info);
    add_bitstring(bits, "00000000000000000100000000", info);
    add_bitstring(bits, "00000000000000000000110000", info);
    add_bitstring(bits, "01000000000000000000000000", info);
    add_bitstring(bits, "00000000000000100100000000", info);
    add_bitstring(bits, "00000000000100000000000000", info);
    add_bitstring(bits, "00000000000001010100000000", info);
    add_bitstring(bits, "00000000001000000000000000", info);
    add_bitstring(bits, "00000000000000000011000000", info);
    add_bitstring(bits, "00000001000000000000000000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(24);
    ts[0] = encrypt_input("0111111111111111111111111111111111111111111111111111111111111111111111111001111111111111111111111111111111111111111111111110111111111111111111111110001111111111111111111110110111111111111111111111011111111111111111111111111111111111111111111111110111111111111111111111010111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111011000", info);
    ts[1] = encrypt_input("11111111111111111111111111", info);
    ts[2] = encrypt_input("00000000000000000000000000", info);
    ts[4] = encrypt_input("10000000011000001011000000", info);
    ts[6] = encrypt_input("00000000000000000000000000", info);
    ts[9] = encrypt_input("00000000000000000000000000", info);
    ts[10] = encrypt_input("00000000000000000000000000", info);
    ts[13] = encrypt_input("00000000000000000000000000", info);
    ts[14] = encrypt_input("00000000000000000000000000", info);
    ts[19] = encrypt_input("00000000000000000000000000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[11];
    ctxt ss[36];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -5, gk, ss[0]); // __s0 = __v0 >> 5
    info.eval->rotate_rows(vs[0], -24, gk, ss[1]); // __s1 = __v0 >> 24
    info.eval->rotate_rows(vs[0], -3, gk, ss[2]); // __s2 = __v0 >> 3
    info.eval->rotate_rows(vs[0], -7, gk, ss[3]); // __s3 = __v0 >> 7
    info.eval->rotate_rows(vs[0], -2, gk, ss[4]); // __s4 = __v0 >> 2
    info.eval->rotate_rows(vs[0], -4, gk, ss[5]); // __s5 = __v0 >> 4
    info.eval->rotate_rows(vs[0], -23, gk, ss[6]); // __s6 = __v0 >> 23
    info.eval->rotate_rows(vs[0], -1, gk, ss[7]); // __s7 = __v0 >> 1
    
    // __t3 = blend(__s6@10001000000000101000000000, __v0@01000000010010000000000000, __s2@00000100000000000000000100, __s5@00000000001000000001000000, __s4@00000000000100000010000010, __s7@00000000000000010000001000, __s0@00000000000000000000110000, __t2@00110011100001000100000001)
    {
    ctxt t3_1, t3_2, t3_3, t3_4, t3_5, t3_6, t3_7;
    info.eval->multiply_plain(ss[6], bits["10001000000000101000000000"], t3_1);
    info.eval->multiply_plain(vs[0], bits["01000000010010000000000000"], t3_2);
    info.eval->multiply_plain(ss[2], bits["00000100000000000000000100"], t3_3);
    info.eval->multiply_plain(ss[5], bits["00000000001000000001000000"], t3_4);
    info.eval->multiply_plain(ss[4], bits["00000000000100000010000010"], t3_5);
    info.eval->multiply_plain(ss[7], bits["00000000000000010000001000"], t3_6);
    info.eval->multiply_plain(ss[0], bits["00000000000000000000110000"], t3_7);
    info.eval->add_many({t3_1, t3_2, t3_3, t3_4, t3_5, t3_6, t3_7, ts[2]}, ts[3]);
    }
    
    info.eval->multiply(ts[1], ts[3], vs[1]); // __v1 = __t1 * __t3
    info.eval->relinearize_inplace(vs[1], rk);
    info.eval->rotate_rows(vs[1], -7, gk, ss[8]); // __s8 = __v1 >> 7
    info.eval->rotate_rows(vs[1], -23, gk, ss[9]); // __s9 = __v1 >> 23
    info.eval->rotate_rows(vs[1], -4, gk, ss[10]); // __s10 = __v1 >> 4
    info.eval->rotate_rows(vs[1], -25, gk, ss[11]); // __s11 = __v1 >> 25
    
    // __t5 = blend(__s1@10000000000000001000000000, __s2@00000000010000000000000000, __s3@00000000001000000000000000, __v0@00000000000000000011000000)
    {
    ctxt t5_1, t5_2, t5_3, t5_4;
    info.eval->multiply_plain(ss[1], bits["10000000000000001000000000"], t5_1);
    info.eval->multiply_plain(ss[2], bits["00000000010000000000000000"], t5_2);
    info.eval->multiply_plain(ss[3], bits["00000000001000000000000000"], t5_3);
    info.eval->multiply_plain(vs[0], bits["00000000000000000011000000"], t5_4);
    info.eval->add_many({t5_1, t5_2, t5_3, t5_4}, ts[5]);
    }
    
    info.eval->multiply(ts[4], ts[5], vs[2]); // __v2 = __t4 * __t5
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -7, gk, ss[12]); // __s12 = __v2 >> 7
    info.eval->rotate_rows(vs[2], -23, gk, ss[13]); // __s13 = __v2 >> 23
    
    // __t7 = blend(__s3@01000000000000000000000000, __s4@00000100000000000000000000, __s6@00000010000000000001000000, __s5@00000001001000000000000000, __s7@00000000100000000000001000, __s2@00000000000000010000000000, __s1@00000000000000001100000000, __s0@00000000000000000000000100, __t6@10011000010111100010010011)
    {
    ctxt t7_1, t7_2, t7_3, t7_4, t7_5, t7_6, t7_7, t7_8;
    info.eval->multiply_plain(ss[3], bits["01000000000000000000000000"], t7_1);
    info.eval->multiply_plain(ss[4], bits["00000100000000000000000000"], t7_2);
    info.eval->multiply_plain(ss[6], bits["00000010000000000001000000"], t7_3);
    info.eval->multiply_plain(ss[5], bits["00000001001000000000000000"], t7_4);
    info.eval->multiply_plain(ss[7], bits["00000000100000000000001000"], t7_5);
    info.eval->multiply_plain(ss[2], bits["00000000000000010000000000"], t7_6);
    info.eval->multiply_plain(ss[1], bits["00000000000000001100000000"], t7_7);
    info.eval->multiply_plain(ss[0], bits["00000000000000000000000100"], t7_8);
    info.eval->add_many({t7_1, t7_2, t7_3, t7_4, t7_5, t7_6, t7_7, t7_8, ts[6]}, ts[7]);
    }
    
    
    // __t8 = blend(__s9@11011000000000100100000000, __v1@00000100000101010010000000, __s13@00000010000000001000000000, __s12@00000001000000000000000000, __s8@00000000110000000001010100, __v2@00000000001000000000000000, __s10@00000000000010000000000001, __s11@00000000000000000000001010)
    {
    ctxt t8_1, t8_2, t8_3, t8_4, t8_5, t8_6, t8_7, t8_8;
    info.eval->multiply_plain(ss[9], bits["11011000000000100100000000"], t8_1);
    info.eval->multiply_plain(vs[1], bits["00000100000101010010000000"], t8_2);
    info.eval->multiply_plain(ss[13], bits["00000010000000001000000000"], t8_3);
    info.eval->multiply_plain(ss[12], bits["00000001000000000000000000"], t8_4);
    info.eval->multiply_plain(ss[8], bits["00000000110000000001010100"], t8_5);
    info.eval->multiply_plain(vs[2], bits["00000000001000000000000000"], t8_6);
    info.eval->multiply_plain(ss[10], bits["00000000000010000000000001"], t8_7);
    info.eval->multiply_plain(ss[11], bits["00000000000000000000001010"], t8_8);
    info.eval->add_many({t8_1, t8_2, t8_3, t8_4, t8_5, t8_6, t8_7, t8_8}, ts[8]);
    }
    
    info.eval->add(ts[7], ts[8], vs[3]); // __v3 = __t7 + __t8
    info.eval->rotate_rows(vs[3], -5, gk, ss[14]); // __s14 = __v3 >> 5
    info.eval->rotate_rows(vs[3], -2, gk, ss[15]); // __s15 = __v3 >> 2
    info.eval->rotate_rows(vs[3], -10, gk, ss[16]); // __s16 = __v3 >> 10
    info.eval->rotate_rows(vs[3], -18, gk, ss[17]); // __s17 = __v3 >> 18
    info.eval->rotate_rows(vs[3], -25, gk, ss[18]); // __s18 = __v3 >> 25
    info.eval->rotate_rows(vs[3], -23, gk, ss[19]); // __s19 = __v3 >> 23
    info.eval->rotate_rows(vs[3], -24, gk, ss[20]); // __s20 = __v3 >> 24
    info.eval->rotate_rows(vs[3], -4, gk, ss[21]); // __s21 = __v3 >> 4
    
    // __t11 = blend(__v3@00001100100000000000000001, __s7@00000001001000000000000000, __s6@00000000000000001000000000, __s4@00000000000000000010000000, __s5@00000000000000000000010000, __s3@00000000000000000000000100, __t9@00000010000000000000001000)
    {
    ctxt t11_1, t11_2, t11_3, t11_4, t11_5, t11_6;
    info.eval->multiply_plain(vs[3], bits["00001100100000000000000001"], t11_1);
    info.eval->multiply_plain(ss[7], bits["00000001001000000000000000"], t11_2);
    info.eval->multiply_plain(ss[6], bits["00000000000000001000000000"], t11_3);
    info.eval->multiply_plain(ss[4], bits["00000000000000000010000000"], t11_4);
    info.eval->multiply_plain(ss[5], bits["00000000000000000000010000"], t11_5);
    info.eval->multiply_plain(ss[3], bits["00000000000000000000000100"], t11_6);
    info.eval->add_many({t11_1, t11_2, t11_3, t11_4, t11_5, t11_6, ts[9]}, ts[11]);
    }
    
    
    // __t12 = blend(__s9@00000010000000001000010000, __s8@00000001000000000000000000, __v1@00000000001000000000001000, __v2@00000000000000000010000000, __s12@00000000000000000000000100, __s3@00000000000000000000000001, __t10@00001100100000000000000000)
    {
    ctxt t12_1, t12_2, t12_3, t12_4, t12_5, t12_6;
    info.eval->multiply_plain(ss[9], bits["00000010000000001000010000"], t12_1);
    info.eval->multiply_plain(ss[8], bits["00000001000000000000000000"], t12_2);
    info.eval->multiply_plain(vs[1], bits["00000000001000000000001000"], t12_3);
    info.eval->multiply_plain(vs[2], bits["00000000000000000010000000"], t12_4);
    info.eval->multiply_plain(ss[12], bits["00000000000000000000000100"], t12_5);
    info.eval->multiply_plain(ss[3], bits["00000000000000000000000001"], t12_6);
    info.eval->add_many({t12_1, t12_2, t12_3, t12_4, t12_5, t12_6, ts[10]}, ts[12]);
    }
    
    info.eval->add(ts[11], ts[12], vs[4]); // __v4 = __t11 + __t12
    info.eval->rotate_rows(vs[4], -5, gk, ss[22]); // __s22 = __v4 >> 5
    info.eval->rotate_rows(vs[4], -23, gk, ss[23]); // __s23 = __v4 >> 23
    info.eval->rotate_rows(vs[4], -2, gk, ss[24]); // __s24 = __v4 >> 2
    info.eval->rotate_rows(vs[4], -18, gk, ss[25]); // __s25 = __v4 >> 18
    info.eval->rotate_rows(vs[4], -25, gk, ss[26]); // __s26 = __v4 >> 25
    info.eval->add(vs[4], ts[13], vs[5]); // __v5 = __v4 + __t13
    info.eval->rotate_rows(vs[5], -23, gk, ss[27]); // __s27 = __v5 >> 23
    
    // __t15 = blend(__s17@00101000000000100000000000, __v3@00010001000100000000000000, __s14@00000110000000000000000000, __s15@00000000100000000000000000, __s24@00000000010000000000100010, __s18@00000000000010000000000100, __s20@00000000000001010100000000, __s16@00000000000000000001000000, __s21@00000000000000000000001001)
    {
    ctxt t15_1, t15_2, t15_3, t15_4, t15_5, t15_6, t15_7, t15_8, t15_9;
    info.eval->multiply_plain(ss[17], bits["00101000000000100000000000"], t15_1);
    info.eval->multiply_plain(vs[3], bits["00010001000100000000000000"], t15_2);
    info.eval->multiply_plain(ss[14], bits["00000110000000000000000000"], t15_3);
    info.eval->multiply_plain(ss[15], bits["00000000100000000000000000"], t15_4);
    info.eval->multiply_plain(ss[24], bits["00000000010000000000100010"], t15_5);
    info.eval->multiply_plain(ss[18], bits["00000000000010000000000100"], t15_6);
    info.eval->multiply_plain(ss[20], bits["00000000000001010100000000"], t15_7);
    info.eval->multiply_plain(ss[16], bits["00000000000000000001000000"], t15_8);
    info.eval->multiply_plain(ss[21], bits["00000000000000000000001001"], t15_9);
    info.eval->add_many({t15_1, t15_2, t15_3, t15_4, t15_5, t15_6, t15_7, t15_8, t15_9}, ts[15]);
    }
    
    
    // __t16 = blend(__v0@00100000000000000000000000, __s0@00000010100100000000000000, __s3@00000000010000100000000000, __s2@00000000000000000100000001, __s7@00000000000000000000100000, __s5@00000000000000000000001010, __t14@00011101000011010001000100)
    {
    ctxt t16_1, t16_2, t16_3, t16_4, t16_5, t16_6;
    info.eval->multiply_plain(vs[0], bits["00100000000000000000000000"], t16_1);
    info.eval->multiply_plain(ss[0], bits["00000010100100000000000000"], t16_2);
    info.eval->multiply_plain(ss[3], bits["00000000010000100000000000"], t16_3);
    info.eval->multiply_plain(ss[2], bits["00000000000000000100000001"], t16_4);
    info.eval->multiply_plain(ss[7], bits["00000000000000000000100000"], t16_5);
    info.eval->multiply_plain(ss[5], bits["00000000000000000000001010"], t16_6);
    info.eval->add_many({t16_1, t16_2, t16_3, t16_4, t16_5, t16_6, ts[14]}, ts[16]);
    }
    
    info.eval->add(ts[15], ts[16], vs[6]); // __v6 = __t15 + __t16
    info.eval->rotate_rows(vs[6], -10, gk, ss[28]); // __s28 = __v6 >> 10
    info.eval->rotate_rows(vs[6], -15, gk, ss[29]); // __s29 = __v6 >> 15
    info.eval->rotate_rows(vs[6], -2, gk, ss[30]); // __s30 = __v6 >> 2
    info.eval->rotate_rows(vs[6], -22, gk, ss[31]); // __s31 = __v6 >> 22
    info.eval->rotate_rows(vs[6], -20, gk, ss[32]); // __s32 = __v6 >> 20
    
    // __t17 = blend(__s23@00000001000000000000000000, __s25@00000000000001000000000000, __s17@00000000000000010000000000, __s21@00000000000000000000100000, __s26@00000000000000000000001000)
    {
    ctxt t17_1, t17_2, t17_3, t17_4, t17_5;
    info.eval->multiply_plain(ss[23], bits["00000001000000000000000000"], t17_1);
    info.eval->multiply_plain(ss[25], bits["00000000000001000000000000"], t17_2);
    info.eval->multiply_plain(ss[17], bits["00000000000000010000000000"], t17_3);
    info.eval->multiply_plain(ss[21], bits["00000000000000000000100000"], t17_4);
    info.eval->multiply_plain(ss[26], bits["00000000000000000000001000"], t17_5);
    info.eval->add_many({t17_1, t17_2, t17_3, t17_4, t17_5}, ts[17]);
    }
    
    
    // __t18 = blend(__s5@00000001000000000000000000, __s7@00000000000001000000000000, __v0@00000000000000010000000000, __s0@00000000000000000000100000, __s2@00000000000000000000001000)
    {
    ctxt t18_1, t18_2, t18_3, t18_4, t18_5;
    info.eval->multiply_plain(ss[5], bits["00000001000000000000000000"], t18_1);
    info.eval->multiply_plain(ss[7], bits["00000000000001000000000000"], t18_2);
    info.eval->multiply_plain(vs[0], bits["00000000000000010000000000"], t18_3);
    info.eval->multiply_plain(ss[0], bits["00000000000000000000100000"], t18_4);
    info.eval->multiply_plain(ss[2], bits["00000000000000000000001000"], t18_5);
    info.eval->add_many({t18_1, t18_2, t18_3, t18_4, t18_5}, ts[18]);
    }
    
    info.eval->add(ts[17], ts[18], vs[7]); // __v7 = __t17 + __t18
    info.eval->rotate_rows(vs[7], -20, gk, ss[33]); // __s33 = __v7 >> 20
    info.eval->rotate_rows(vs[7], -15, gk, ss[34]); // __s34 = __v7 >> 15
    info.eval->rotate_rows(vs[7], -22, gk, ss[35]); // __s35 = __v7 >> 22
    info.eval->add(ss[27], ss[22], vs[8]); // __v8 = __s27 + __s22
    
    // __t20 = blend(__s33@01000000000000000000000000, __s29@00100000000000000010000000, __v4@00000010000000000000000000, __s30@00000000010000000001000000, __s19@00000000000100000000000000, __s28@00000000000010000000000000, __s32@00000000000000100100000000, __s31@00000000000000010000010000, __s35@00000000000000001000000000, __s26@00000000000000000000000010)
    {
    ctxt t20_1, t20_2, t20_3, t20_4, t20_5, t20_6, t20_7, t20_8, t20_9, t20_10;
    info.eval->multiply_plain(ss[33], bits["01000000000000000000000000"], t20_1);
    info.eval->multiply_plain(ss[29], bits["00100000000000000010000000"], t20_2);
    info.eval->multiply_plain(vs[4], bits["00000010000000000000000000"], t20_3);
    info.eval->multiply_plain(ss[30], bits["00000000010000000001000000"], t20_4);
    info.eval->multiply_plain(ss[19], bits["00000000000100000000000000"], t20_5);
    info.eval->multiply_plain(ss[28], bits["00000000000010000000000000"], t20_6);
    info.eval->multiply_plain(ss[32], bits["00000000000000100100000000"], t20_7);
    info.eval->multiply_plain(ss[31], bits["00000000000000010000010000"], t20_8);
    info.eval->multiply_plain(ss[35], bits["00000000000000001000000000"], t20_9);
    info.eval->multiply_plain(ss[26], bits["00000000000000000000000010"], t20_10);
    info.eval->add_many({t20_1, t20_2, t20_3, t20_4, t20_5, t20_6, t20_7, t20_8, t20_9, t20_10}, ts[20]);
    }
    
    
    // __t21 = blend(__s31@01000000000000000010000000, __s23@00100000000000000000000000, __v0@00000010000000000000000000, __s22@00000000010000000000000000, __v6@00000000000010100000000010, __v7@00000000000000010000000000, __s28@00000000000000001001010000, __s30@00000000000000000100000000, __t19@00000000000100000000000000)
    {
    ctxt t21_1, t21_2, t21_3, t21_4, t21_5, t21_6, t21_7, t21_8;
    info.eval->multiply_plain(ss[31], bits["01000000000000000010000000"], t21_1);
    info.eval->multiply_plain(ss[23], bits["00100000000000000000000000"], t21_2);
    info.eval->multiply_plain(vs[0], bits["00000010000000000000000000"], t21_3);
    info.eval->multiply_plain(ss[22], bits["00000000010000000000000000"], t21_4);
    info.eval->multiply_plain(vs[6], bits["00000000000010100000000010"], t21_5);
    info.eval->multiply_plain(vs[7], bits["00000000000000010000000000"], t21_6);
    info.eval->multiply_plain(ss[28], bits["00000000000000001001010000"], t21_7);
    info.eval->multiply_plain(ss[30], bits["00000000000000000100000000"], t21_8);
    info.eval->add_many({t21_1, t21_2, t21_3, t21_4, t21_5, t21_6, t21_7, t21_8, ts[19]}, ts[21]);
    }
    
    info.eval->add(ts[20], ts[21], vs[9]); // __v9 = __t20 + __t21
    
    // __t22 = blend(__s34@00100000000000000000000000, __v9@00000010000100000000000000)
    {
    ctxt t22_1, t22_2;
    info.eval->multiply_plain(ss[34], bits["00100000000000000000000000"], t22_1);
    info.eval->multiply_plain(vs[9], bits["00000010000100000000000000"], t22_2);
    info.eval->add(t22_1, t22_2, ts[22]);
    }
    
    
    // __t23 = blend(__s32@00100000000000000000000000, __s30@00000010000000000000000000, __s34@00000000000100000000000000)
    {
    ctxt t23_1, t23_2, t23_3;
    info.eval->multiply_plain(ss[32], bits["00100000000000000000000000"], t23_1);
    info.eval->multiply_plain(ss[30], bits["00000010000000000000000000"], t23_2);
    info.eval->multiply_plain(ss[34], bits["00000000000100000000000000"], t23_3);
    info.eval->add_many({t23_1, t23_2, t23_3}, ts[23]);
    }
    
    info.eval->add(ts[22], ts[23], vs[10]); // __v10 = __t22 + __t23
    return vs[10];
}
    