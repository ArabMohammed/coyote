
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "000000001000000000", info);
    add_bitstring(bits, "000000000000000100", info);
    add_bitstring(bits, "001001010000000000", info);
    add_bitstring(bits, "100000000000000000", info);
    add_bitstring(bits, "000000010000000000", info);
    add_bitstring(bits, "000100001000000001", info);
    add_bitstring(bits, "000001010010000000", info);
    add_bitstring(bits, "001001010101011100", info);
    add_bitstring(bits, "001000000010001000", info);
    add_bitstring(bits, "001000000000000000", info);
    add_bitstring(bits, "000100010000000000", info);
    add_bitstring(bits, "100110100100000000", info);
    add_bitstring(bits, "000000000000010000", info);
    add_bitstring(bits, "000000000010001100", info);
    add_bitstring(bits, "010100100010100000", info);
    add_bitstring(bits, "010010100101100100", info);
    add_bitstring(bits, "000100000000000000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(9);
    ts[0] = encrypt_input("011111111111111111111111110110011111111111111111111111110110111111111111111111111111111111111111111111111111111110100111111111111111111111111111111111111111111111111111111101111111111111111111111111110111111111111111111111111111100111111111111111111111111111100", info);
    ts[1] = encrypt_input("000000000000000000", info);
    ts[2] = encrypt_input("000000000000000000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[8];
    ctxt ss[9];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -2, gk, ss[0]); // __s0 = __v0 >> 2
    info.eval->rotate_rows(vs[0], -14, gk, ss[1]); // __s1 = __v0 >> 14
    info.eval->rotate_rows(vs[0], -12, gk, ss[2]); // __s2 = __v0 >> 12
    info.eval->rotate_rows(vs[0], -16, gk, ss[3]); // __s3 = __v0 >> 16
    
    // __t3 = blend(__s1@100000000000000000, __v0@010010100101100100, __s0@000100001000000001, __s3@000001010010000000, __t1@001000000000010010)
    {
    ctxt t3_1, t3_2, t3_3, t3_4;
    info.eval->multiply_plain(ss[1], bits["100000000000000000"], t3_1);
    info.eval->multiply_plain(vs[0], bits["010010100101100100"], t3_2);
    info.eval->multiply_plain(ss[0], bits["000100001000000001"], t3_3);
    info.eval->multiply_plain(ss[3], bits["000001010010000000"], t3_4);
    info.eval->add_many({t3_1, t3_2, t3_3, t3_4, ts[1]}, ts[3]);
    }
    
    
    // __t4 = blend(__s2@100110100100000000, __s1@001001010000000000, __s3@000000000000010000, __t2@010000001011100111)
    {
    ctxt t4_1, t4_2, t4_3;
    info.eval->multiply_plain(ss[2], bits["100110100100000000"], t4_1);
    info.eval->multiply_plain(ss[1], bits["001001010000000000"], t4_2);
    info.eval->multiply_plain(ss[3], bits["000000000000010000"], t4_3);
    info.eval->add_many({t4_1, t4_2, t4_3, ts[2]}, ts[4]);
    }
    
    info.eval->add(ts[3], ts[4], vs[1]); // __v1 = __t3 + __t4
    info.eval->rotate_rows(vs[1], -16, gk, ss[4]); // __s4 = __v1 >> 16
    info.eval->multiply(vs[1], vs[1], vs[2]); // __v2 = __v1 * __v1
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -16, gk, ss[5]); // __s5 = __v2 >> 16
    
    // __t5 = blend(__v1@010100100010100000, __s4@001001010101011100)
    {
    ctxt t5_1, t5_2;
    info.eval->multiply_plain(vs[1], bits["010100100010100000"], t5_1);
    info.eval->multiply_plain(ss[4], bits["001001010101011100"], t5_2);
    info.eval->add(t5_1, t5_2, ts[5]);
    }
    
    
    // __t6 = blend(__v1@010100100010100000, __s4@001001010101011100)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(vs[1], bits["010100100010100000"], t6_1);
    info.eval->multiply_plain(ss[4], bits["001001010101011100"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    info.eval->multiply(ts[5], ts[6], vs[3]); // __v3 = __t5 * __t6
    info.eval->relinearize_inplace(vs[3], rk);
    info.eval->rotate_rows(vs[3], -1, gk, ss[6]); // __s6 = __v3 >> 1
    info.eval->rotate_rows(vs[3], -15, gk, ss[7]); // __s7 = __v3 >> 15
    info.eval->rotate_rows(vs[3], -3, gk, ss[8]); // __s8 = __v3 >> 3
    info.eval->add(ss[3], ss[1], vs[4]); // __v4 = __s3 + __s1
    info.eval->multiply(vs[4], vs[4], vs[5]); // __v5 = __v4 * __v4
    info.eval->relinearize_inplace(vs[5], rk);
    
    // __t7 = blend(__v2@100000000000000000, __s6@001000000010001000, __s5@000100000000000000, __v3@000000010000000000, __v5@000000001000000000, __s8@000000000000000100)
    {
    ctxt t7_1, t7_2, t7_3, t7_4, t7_5, t7_6;
    info.eval->multiply_plain(vs[2], bits["100000000000000000"], t7_1);
    info.eval->multiply_plain(ss[6], bits["001000000010001000"], t7_2);
    info.eval->multiply_plain(ss[5], bits["000100000000000000"], t7_3);
    info.eval->multiply_plain(vs[3], bits["000000010000000000"], t7_4);
    info.eval->multiply_plain(vs[5], bits["000000001000000000"], t7_5);
    info.eval->multiply_plain(ss[8], bits["000000000000000100"], t7_6);
    info.eval->add_many({t7_1, t7_2, t7_3, t7_4, t7_5, t7_6}, ts[7]);
    }
    
    
    // __t8 = blend(__s7@100000000000000000, __v2@001000000000000000, __s6@000100010000000000, __s8@000000001000000000, __v3@000000000010001100)
    {
    ctxt t8_1, t8_2, t8_3, t8_4, t8_5;
    info.eval->multiply_plain(ss[7], bits["100000000000000000"], t8_1);
    info.eval->multiply_plain(vs[2], bits["001000000000000000"], t8_2);
    info.eval->multiply_plain(ss[6], bits["000100010000000000"], t8_3);
    info.eval->multiply_plain(ss[8], bits["000000001000000000"], t8_4);
    info.eval->multiply_plain(vs[3], bits["000000000010001100"], t8_5);
    info.eval->add_many({t8_1, t8_2, t8_3, t8_4, t8_5}, ts[8]);
    }
    
    info.eval->add(ts[7], ts[8], vs[6]); // __v6 = __t7 + __t8
    info.eval->add(vs[2], ss[7], vs[7]); // __v7 = __v2 + __s7
    return vs[7];
}
    