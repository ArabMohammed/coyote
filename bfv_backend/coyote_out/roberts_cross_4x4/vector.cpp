
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "00000000000000010000000010000000", info);
    add_bitstring(bits, "00100000010100000000000000000000", info);
    add_bitstring(bits, "00000010000000000000000000000000", info);
    add_bitstring(bits, "00000000000000000000000001000000", info);
    add_bitstring(bits, "00010000000100000000000000001000", info);
    add_bitstring(bits, "00010000000001000010000000110000", info);
    add_bitstring(bits, "00001000100000010000110000010000", info);
    add_bitstring(bits, "00000000000000000000010000000000", info);
    add_bitstring(bits, "00000010000000000001000000001000", info);
    add_bitstring(bits, "00000000000000010000010010000000", info);
    add_bitstring(bits, "00000000000000000010000000000001", info);
    add_bitstring(bits, "00000000000000000000000000000010", info);
    add_bitstring(bits, "00000001000000001100000000000000", info);
    add_bitstring(bits, "00000000000000000000100000000000", info);
    add_bitstring(bits, "00000000000100010100000000000000", info);
    add_bitstring(bits, "00000000000000100000000000000000", info);
    add_bitstring(bits, "00001000000000000000001100000000", info);
    add_bitstring(bits, "00010000001000000000000000000000", info);
    add_bitstring(bits, "00110001000000000000000000000000", info);
    add_bitstring(bits, "00100000000000000000000000000000", info);
    add_bitstring(bits, "00000000000000000000001000000000", info);
    add_bitstring(bits, "00000000000000000000000000000111", info);
    add_bitstring(bits, "00000000001000000000000000000000", info);
    add_bitstring(bits, "00000000000000000000000000010000", info);
    add_bitstring(bits, "00000011000001000000000000000000", info);
    add_bitstring(bits, "00000100000000000000000000000000", info);
    add_bitstring(bits, "00000000000000000001000000000000", info);
    add_bitstring(bits, "00100000000000001000001001000010", info);
    add_bitstring(bits, "00000000000001000001000000101000", info);
    add_bitstring(bits, "10000000000010010000010000000000", info);
    add_bitstring(bits, "00000000000000000000000110000000", info);
    add_bitstring(bits, "10000000010000000000000010010000", info);
    add_bitstring(bits, "01000000000100000001000000000000", info);
    add_bitstring(bits, "00000000000000000100000000000000", info);
    add_bitstring(bits, "10000000010000000000010000000010", info);
    add_bitstring(bits, "00000000000000101000000000000000", info);
    add_bitstring(bits, "00000000000000000010000001000000", info);
    add_bitstring(bits, "00000000000001000000000000000000", info);
    add_bitstring(bits, "10001101011010100100100000001000", info);
    add_bitstring(bits, "00000010000000000000000100000101", info);
    add_bitstring(bits, "00000000000100000000000000100000", info);
    add_bitstring(bits, "00010011000000000000000000000000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(9);
    ts[0] = encrypt_input("00111111111111111111111111101100111111111111111111111111111111111111111111111111111110110001111111111111111111111111110111111111111111111111111101001111111111111111111111111111011111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111111111111111011111111111111111111111111111100111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111000011111111111111111111111111100", info);
    ts[1] = encrypt_input("00000000000000000000000000000000", info);
    ts[2] = encrypt_input("00000000000000000000000000000000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[5];
    ctxt ss[23];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -1, gk, ss[0]); // __s0 = __v0 >> 1
    info.eval->rotate_rows(vs[0], -2, gk, ss[1]); // __s1 = __v0 >> 2
    info.eval->rotate_rows(vs[0], -8, gk, ss[2]); // __s2 = __v0 >> 8
    info.eval->rotate_rows(vs[0], -27, gk, ss[3]); // __s3 = __v0 >> 27
    info.eval->rotate_rows(vs[0], -31, gk, ss[4]); // __s4 = __v0 >> 31
    info.eval->rotate_rows(vs[0], -24, gk, ss[5]); // __s5 = __v0 >> 24
    info.eval->rotate_rows(vs[0], -25, gk, ss[6]); // __s6 = __v0 >> 25
    info.eval->rotate_rows(vs[0], -29, gk, ss[7]); // __s7 = __v0 >> 29
    info.eval->rotate_rows(vs[0], -7, gk, ss[8]); // __s8 = __v0 >> 7
    info.eval->rotate_rows(vs[0], -28, gk, ss[9]); // __s9 = __v0 >> 28
    info.eval->rotate_rows(vs[0], -17, gk, ss[10]); // __s10 = __v0 >> 17
    info.eval->rotate_rows(vs[0], -5, gk, ss[11]); // __s11 = __v0 >> 5
    
    // __t3 = blend(__s5@00100000010100000000000000000000, __s0@00010011000000000000000000000000, __s1@00001000100000010000110000010000, __s10@00000100000000000000000000000000, __s7@00000000001000000000000000000000, __s2@00000000000001000001000000101000, __s4@00000000000000101000000000000000, __s3@00000000000000000010000001000000, __s8@00000000000000000000001000000000, __v0@00000000000000000000000110000000, __s11@00000000000000000000000000000111, __t1@11000000000010000100000000000000)
    {
    ctxt t3_1, t3_2, t3_3, t3_4, t3_5, t3_6, t3_7, t3_8, t3_9, t3_10, t3_11;
    info.eval->multiply_plain(ss[5], bits["00100000010100000000000000000000"], t3_1);
    info.eval->multiply_plain(ss[0], bits["00010011000000000000000000000000"], t3_2);
    info.eval->multiply_plain(ss[1], bits["00001000100000010000110000010000"], t3_3);
    info.eval->multiply_plain(ss[10], bits["00000100000000000000000000000000"], t3_4);
    info.eval->multiply_plain(ss[7], bits["00000000001000000000000000000000"], t3_5);
    info.eval->multiply_plain(ss[2], bits["00000000000001000001000000101000"], t3_6);
    info.eval->multiply_plain(ss[4], bits["00000000000000101000000000000000"], t3_7);
    info.eval->multiply_plain(ss[3], bits["00000000000000000010000001000000"], t3_8);
    info.eval->multiply_plain(ss[8], bits["00000000000000000000001000000000"], t3_9);
    info.eval->multiply_plain(vs[0], bits["00000000000000000000000110000000"], t3_10);
    info.eval->multiply_plain(ss[11], bits["00000000000000000000000000000111"], t3_11);
    info.eval->add_many({t3_1, t3_2, t3_3, t3_4, t3_5, t3_6, t3_7, t3_8, t3_9, t3_10, t3_11, ts[1]}, ts[3]);
    }
    
    
    // __t4 = blend(__s3@10000000000010010000010000000000, __s10@00010000001000000000000000000000, __s4@00001000000000000000001100000000, __s6@00000010000000000000000000000000, __s5@00000001000000001100000000000000, __s9@00000000000100000000000000100000, __s7@00000000000000100000000000000000, __s2@00000000000000000010000000000001, __s0@00000000000000000001000000000000, __s8@00000000000000000000100000000000, __s11@00000000000000000000000001000000, __t2@01100100110001000000000010011110)
    {
    ctxt t4_1, t4_2, t4_3, t4_4, t4_5, t4_6, t4_7, t4_8, t4_9, t4_10, t4_11;
    info.eval->multiply_plain(ss[3], bits["10000000000010010000010000000000"], t4_1);
    info.eval->multiply_plain(ss[10], bits["00010000001000000000000000000000"], t4_2);
    info.eval->multiply_plain(ss[4], bits["00001000000000000000001100000000"], t4_3);
    info.eval->multiply_plain(ss[6], bits["00000010000000000000000000000000"], t4_4);
    info.eval->multiply_plain(ss[5], bits["00000001000000001100000000000000"], t4_5);
    info.eval->multiply_plain(ss[9], bits["00000000000100000000000000100000"], t4_6);
    info.eval->multiply_plain(ss[7], bits["00000000000000100000000000000000"], t4_7);
    info.eval->multiply_plain(ss[2], bits["00000000000000000010000000000001"], t4_8);
    info.eval->multiply_plain(ss[0], bits["00000000000000000001000000000000"], t4_9);
    info.eval->multiply_plain(ss[8], bits["00000000000000000000100000000000"], t4_10);
    info.eval->multiply_plain(ss[11], bits["00000000000000000000000001000000"], t4_11);
    info.eval->add_many({t4_1, t4_2, t4_3, t4_4, t4_5, t4_6, t4_7, t4_8, t4_9, t4_10, t4_11, ts[2]}, ts[4]);
    }
    
    info.eval->add(ts[3], ts[4], vs[1]); // __v1 = __t3 + __t4
    info.eval->rotate_rows(vs[1], -15, gk, ss[12]); // __s12 = __v1 >> 15
    info.eval->rotate_rows(vs[1], -5, gk, ss[13]); // __s13 = __v1 >> 5
    info.eval->rotate_rows(vs[1], -29, gk, ss[14]); // __s14 = __v1 >> 29
    info.eval->rotate_rows(vs[1], -31, gk, ss[15]); // __s15 = __v1 >> 31
    info.eval->rotate_rows(vs[1], -22, gk, ss[16]); // __s16 = __v1 >> 22
    
    // __t5 = blend(__s14@10001101011010100100100000001000, __s16@01000000000100000001000000000000, __v1@00100000000000001000001001000010, __s15@00010000000001000010000000110000, __s13@00000010000000000000000100000101, __s12@00000000000000010000010010000000)
    {
    ctxt t5_1, t5_2, t5_3, t5_4, t5_5, t5_6;
    info.eval->multiply_plain(ss[14], bits["10001101011010100100100000001000"], t5_1);
    info.eval->multiply_plain(ss[16], bits["01000000000100000001000000000000"], t5_2);
    info.eval->multiply_plain(vs[1], bits["00100000000000001000001001000010"], t5_3);
    info.eval->multiply_plain(ss[15], bits["00010000000001000010000000110000"], t5_4);
    info.eval->multiply_plain(ss[13], bits["00000010000000000000000100000101"], t5_5);
    info.eval->multiply_plain(ss[12], bits["00000000000000010000010010000000"], t5_6);
    info.eval->add_many({t5_1, t5_2, t5_3, t5_4, t5_5, t5_6}, ts[5]);
    }
    
    
    // __t6 = blend(__s14@10001101011010100100100000001000, __s16@01000000000100000001000000000000, __v1@00100000000000001000001001000010, __s15@00010000000001000010000000110000, __s13@00000010000000000000000100000101, __s12@00000000000000010000010010000000)
    {
    ctxt t6_1, t6_2, t6_3, t6_4, t6_5, t6_6;
    info.eval->multiply_plain(ss[14], bits["10001101011010100100100000001000"], t6_1);
    info.eval->multiply_plain(ss[16], bits["01000000000100000001000000000000"], t6_2);
    info.eval->multiply_plain(vs[1], bits["00100000000000001000001001000010"], t6_3);
    info.eval->multiply_plain(ss[15], bits["00010000000001000010000000110000"], t6_4);
    info.eval->multiply_plain(ss[13], bits["00000010000000000000000100000101"], t6_5);
    info.eval->multiply_plain(ss[12], bits["00000000000000010000010010000000"], t6_6);
    info.eval->add_many({t6_1, t6_2, t6_3, t6_4, t6_5, t6_6}, ts[6]);
    }
    
    info.eval->multiply(ts[5], ts[6], vs[2]); // __v2 = __t5 * __t6
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -31, gk, ss[17]); // __s17 = __v2 >> 31
    info.eval->rotate_rows(vs[2], -1, gk, ss[18]); // __s18 = __v2 >> 1
    info.eval->rotate_rows(vs[2], -11, gk, ss[19]); // __s19 = __v2 >> 11
    info.eval->rotate_rows(vs[2], -8, gk, ss[20]); // __s20 = __v2 >> 8
    info.eval->rotate_rows(vs[2], -23, gk, ss[21]); // __s21 = __v2 >> 23
    info.eval->rotate_rows(vs[2], -28, gk, ss[22]); // __s22 = __v2 >> 28
    info.eval->multiply(ss[14], ss[14], vs[3]); // __v3 = __s14 * __s14
    info.eval->relinearize_inplace(vs[3], rk);
    
    // __t7 = blend(__s17@10000000010000000000000010010000, __v2@00110001000000000000000000000000, __s18@00000010000000000001000000001000, __s21@00000000000100010100000000000000, __s22@00000000000001000000000000000000, __s20@00000000000000000000010000000000, __s19@00000000000000000000000000000010)
    {
    ctxt t7_1, t7_2, t7_3, t7_4, t7_5, t7_6, t7_7;
    info.eval->multiply_plain(ss[17], bits["10000000010000000000000010010000"], t7_1);
    info.eval->multiply_plain(vs[2], bits["00110001000000000000000000000000"], t7_2);
    info.eval->multiply_plain(ss[18], bits["00000010000000000001000000001000"], t7_3);
    info.eval->multiply_plain(ss[21], bits["00000000000100010100000000000000"], t7_4);
    info.eval->multiply_plain(ss[22], bits["00000000000001000000000000000000"], t7_5);
    info.eval->multiply_plain(ss[20], bits["00000000000000000000010000000000"], t7_6);
    info.eval->multiply_plain(ss[19], bits["00000000000000000000000000000010"], t7_7);
    info.eval->add_many({t7_1, t7_2, t7_3, t7_4, t7_5, t7_6, t7_7}, ts[7]);
    }
    
    
    // __t8 = blend(__v2@10000000010000000000010000000010, __v3@00100000000000000000000000000000, __s17@00010000000100000000000000001000, __s21@00000011000001000000000000000000, __s18@00000000000000010000000010000000, __s19@00000000000000000100000000000000, __s20@00000000000000000001000000000000, __s22@00000000000000000000000000010000)
    {
    ctxt t8_1, t8_2, t8_3, t8_4, t8_5, t8_6, t8_7, t8_8;
    info.eval->multiply_plain(vs[2], bits["10000000010000000000010000000010"], t8_1);
    info.eval->multiply_plain(vs[3], bits["00100000000000000000000000000000"], t8_2);
    info.eval->multiply_plain(ss[17], bits["00010000000100000000000000001000"], t8_3);
    info.eval->multiply_plain(ss[21], bits["00000011000001000000000000000000"], t8_4);
    info.eval->multiply_plain(ss[18], bits["00000000000000010000000010000000"], t8_5);
    info.eval->multiply_plain(ss[19], bits["00000000000000000100000000000000"], t8_6);
    info.eval->multiply_plain(ss[20], bits["00000000000000000001000000000000"], t8_7);
    info.eval->multiply_plain(ss[22], bits["00000000000000000000000000010000"], t8_8);
    info.eval->add_many({t8_1, t8_2, t8_3, t8_4, t8_5, t8_6, t8_7, t8_8}, ts[8]);
    }
    
    info.eval->add(ts[7], ts[8], vs[4]); // __v4 = __t7 + __t8
    return vs[4];
}
    