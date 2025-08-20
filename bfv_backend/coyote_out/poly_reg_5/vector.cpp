
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "00000000000000000000000011", info);
    add_bitstring(bits, "00010010000000100010000000", info);
    add_bitstring(bits, "01000000000000001000001100", info);
    add_bitstring(bits, "00000000000000100000110000", info);
    add_bitstring(bits, "01000000100000000000000000", info);
    add_bitstring(bits, "00000000001001000000000000", info);
    add_bitstring(bits, "00000000000001000000001000", info);
    add_bitstring(bits, "00000000100000000000000000", info);
    add_bitstring(bits, "00000000000000000010000100", info);
    add_bitstring(bits, "00000000000000000000001100", info);
    add_bitstring(bits, "01000000100000100000000000", info);
    add_bitstring(bits, "00000000100000100000000011", info);
    add_bitstring(bits, "00000000000000001000000000", info);
    add_bitstring(bits, "01000000000000000000000000", info);
    add_bitstring(bits, "00000010000000000000000000", info);
    add_bitstring(bits, "00010000001000100000110000", info);
    add_bitstring(bits, "00000000000000000000111100", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(13);
    ts[0] = encrypt_input("11111111111101100011111111111101100111111111111011111111111111101100111111111111011101111111111110111111111111111011101111111111110111011111111111101110011111111111101111111111111110111111111111111011111111111111101111111111111110111111111111111011100", info);
    ts[1] = encrypt_input("01111111111111110000000111111111111111100000111111111111111100000000011111111111111111111111111111111", info);
    ts[2] = encrypt_input("00000000111111111111111100000111111111111111100011111111111111100000011111111111111111111111111111111", info);
    ts[3] = encrypt_input("00011111111111111100000001111111111111111000111111111111111100000111111111111111111111111111111110000", info);
    ts[4] = encrypt_input("01111111111111110000011111111111111110000000001111111111111111000001111111111111111111111111111111100", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[10];
    ctxt ss[8];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -3, gk, ss[0]); // __s0 = __v0 >> 3
    info.eval->rotate_rows(vs[0], -5, gk, ss[1]); // __s1 = __v0 >> 5
    vs[1] = ts[1];
    vs[2] = ts[2];
    info.eval->rotate_rows(vs[2], -9, gk, ss[2]); // __s2 = __v2 >> 9
    vs[3] = ts[3];
    vs[4] = ts[4];
    info.eval->rotate_rows(vs[4], -2, gk, ss[3]); // __s3 = __v4 >> 2
    
    // __t5 = blend(__s0@00010010000000100010000000, __s1@00000000001001000000000000, __v0@00000000000000000000111100)
    {
    ctxt t5_1, t5_2, t5_3;
    info.eval->multiply_plain(ss[0], bits["00010010000000100010000000"], t5_1);
    info.eval->multiply_plain(ss[1], bits["00000000001001000000000000"], t5_2);
    info.eval->multiply_plain(vs[0], bits["00000000000000000000111100"], t5_3);
    info.eval->add_many({t5_1, t5_2, t5_3}, ts[5]);
    }
    
    
    // __t6 = blend(__v3@00010000001000100000110000, __v0@00000010000000000000000000, __s0@00000000000001000000001000, __s1@00000000000000000010000100)
    {
    ctxt t6_1, t6_2, t6_3, t6_4;
    info.eval->multiply_plain(vs[3], bits["00010000001000100000110000"], t6_1);
    info.eval->multiply_plain(vs[0], bits["00000010000000000000000000"], t6_2);
    info.eval->multiply_plain(ss[0], bits["00000000000001000000001000"], t6_3);
    info.eval->multiply_plain(ss[1], bits["00000000000000000010000100"], t6_4);
    info.eval->add_many({t6_1, t6_2, t6_3, t6_4}, ts[6]);
    }
    
    info.eval->multiply(ts[5], ts[6], vs[5]); // __v5 = __t5 * __t6
    info.eval->relinearize_inplace(vs[5], rk);
    info.eval->rotate_rows(vs[5], -24, gk, ss[4]); // __s4 = __v5 >> 24
    info.eval->rotate_rows(vs[5], -21, gk, ss[5]); // __s5 = __v5 >> 21
    
    // __t7 = blend(__s5@01000000100000000000000000, __s4@00000000000000001000000000, __v5@00000000000000000000001100)
    {
    ctxt t7_1, t7_2, t7_3;
    info.eval->multiply_plain(ss[5], bits["01000000100000000000000000"], t7_1);
    info.eval->multiply_plain(ss[4], bits["00000000000000001000000000"], t7_2);
    info.eval->multiply_plain(vs[5], bits["00000000000000000000001100"], t7_3);
    info.eval->add_many({t7_1, t7_2, t7_3}, ts[7]);
    }
    
    
    // __t8 = blend(__v4@01000000000000001000001100, __s3@00000000100000000000000000)
    {
    ctxt t8_1, t8_2;
    info.eval->multiply_plain(vs[4], bits["01000000000000001000001100"], t8_1);
    info.eval->multiply_plain(ss[3], bits["00000000100000000000000000"], t8_2);
    info.eval->add(t8_1, t8_2, ts[8]);
    }
    
    info.eval->multiply(ts[7], ts[8], vs[6]); // __v6 = __t7 * __t8
    info.eval->relinearize_inplace(vs[6], rk);
    info.eval->rotate_rows(vs[6], -24, gk, ss[6]); // __s6 = __v6 >> 24
    
    // __t9 = blend(__v6@01000000100000000000000000, __s6@00000000000000100000110000)
    {
    ctxt t9_1, t9_2;
    info.eval->multiply_plain(vs[6], bits["01000000100000000000000000"], t9_1);
    info.eval->multiply_plain(ss[6], bits["00000000000000100000110000"], t9_2);
    info.eval->add(t9_1, t9_2, ts[9]);
    }
    
    
    // __t10 = blend(__s4@01000000100000000000000000, __v5@00000000000000100000110000)
    {
    ctxt t10_1, t10_2;
    info.eval->multiply_plain(ss[4], bits["01000000100000000000000000"], t10_1);
    info.eval->multiply_plain(vs[5], bits["00000000000000100000110000"], t10_2);
    info.eval->add(t10_1, t10_2, ts[10]);
    }
    
    info.eval->add(ts[9], ts[10], vs[7]); // __v7 = __t9 + __t10
    info.eval->rotate_rows(vs[7], -4, gk, ss[7]); // __s7 = __v7 >> 4
    
    // __t11 = blend(__v7@01000000100000100000000000, __s7@00000000000000000000000011)
    {
    ctxt t11_1, t11_2;
    info.eval->multiply_plain(vs[7], bits["01000000100000100000000000"], t11_1);
    info.eval->multiply_plain(ss[7], bits["00000000000000000000000011"], t11_2);
    info.eval->add(t11_1, t11_2, ts[11]);
    }
    
    
    // __t12 = blend(__s2@01000000000000000000000000, __v2@00000000100000100000000011)
    {
    ctxt t12_1, t12_2;
    info.eval->multiply_plain(ss[2], bits["01000000000000000000000000"], t12_1);
    info.eval->multiply_plain(vs[2], bits["00000000100000100000000011"], t12_2);
    info.eval->add(t12_1, t12_2, ts[12]);
    }
    
    info.eval->add(ts[11], ts[12], vs[8]); // __v8 = __t11 + __t12
    info.eval->sub(vs[1], vs[8], vs[9]); // __v9 = __v1 - __v8
    return vs[9];
}
    