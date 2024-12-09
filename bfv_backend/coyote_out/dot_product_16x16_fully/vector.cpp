
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "0000000000100000", info);
    add_bitstring(bits, "0000010000000000", info);
    add_bitstring(bits, "0010000000000000", info);
    add_bitstring(bits, "0001000000000000", info);
    add_bitstring(bits, "0001000010000000", info);
    add_bitstring(bits, "0000000000001000", info);
    add_bitstring(bits, "0100010000111000", info);
    add_bitstring(bits, "0000000000101000", info);
    add_bitstring(bits, "0000000010000000", info);
    add_bitstring(bits, "0100000000000000", info);
    add_bitstring(bits, "0000000000011000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(8);
    ts[0] = encrypt_input("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111", info);
    ts[1] = encrypt_input("11111111111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[8];
    ctxt ss[10];

    vs[0] = ts[0];
    vs[1] = ts[1];
    info.eval->multiply(vs[0], vs[1], vs[2]); // __v2 = __v0 * __v1
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -3, gk, ss[0]); // __s0 = __v2 >> 3
    info.eval->rotate_rows(vs[2], -14, gk, ss[1]); // __s1 = __v2 >> 14
    info.eval->rotate_rows(vs[2], -15, gk, ss[2]); // __s2 = __v2 >> 15
    info.eval->rotate_rows(vs[2], -2, gk, ss[3]); // __s3 = __v2 >> 2
    
    // __t2 = blend(__s3@0100000000000000, __v2@0001000010000000, __s2@0000010000000000, __s0@0000000000100000, __s1@0000000000011000)
    {
    ctxt t2_1, t2_2, t2_3, t2_4, t2_5;
    info.eval->multiply_plain(ss[3], bits["0100000000000000"], t2_1);
    info.eval->multiply_plain(vs[2], bits["0001000010000000"], t2_2);
    info.eval->multiply_plain(ss[2], bits["0000010000000000"], t2_3);
    info.eval->multiply_plain(ss[0], bits["0000000000100000"], t2_4);
    info.eval->multiply_plain(ss[1], bits["0000000000011000"], t2_5);
    info.eval->add_many({t2_1, t2_2, t2_3, t2_4, t2_5}, ts[2]);
    }
    
    
    // __t3 = blend(__v2@0100010000111000, __s0@0001000000000000, __s2@0000000010000000)
    {
    ctxt t3_1, t3_2, t3_3;
    info.eval->multiply_plain(vs[2], bits["0100010000111000"], t3_1);
    info.eval->multiply_plain(ss[0], bits["0001000000000000"], t3_2);
    info.eval->multiply_plain(ss[2], bits["0000000010000000"], t3_3);
    info.eval->add_many({t3_1, t3_2, t3_3}, ts[3]);
    }
    
    info.eval->add(ts[2], ts[3], vs[3]); // __v3 = __t2 + __t3
    info.eval->rotate_rows(vs[3], -2, gk, ss[4]); // __s4 = __v3 >> 2
    info.eval->rotate_rows(vs[3], -13, gk, ss[5]); // __s5 = __v3 >> 13
    info.eval->rotate_rows(vs[3], -1, gk, ss[6]); // __s6 = __v3 >> 1
    
    // __t4 = blend(__v2@0010000000000000, __v3@0001000000000000, __s4@0000000000100000, __s6@0000000000001000)
    {
    ctxt t4_1, t4_2, t4_3, t4_4;
    info.eval->multiply_plain(vs[2], bits["0010000000000000"], t4_1);
    info.eval->multiply_plain(vs[3], bits["0001000000000000"], t4_2);
    info.eval->multiply_plain(ss[4], bits["0000000000100000"], t4_3);
    info.eval->multiply_plain(ss[6], bits["0000000000001000"], t4_4);
    info.eval->add_many({t4_1, t4_2, t4_3, t4_4}, ts[4]);
    }
    
    
    // __t5 = blend(__s1@0010000000000000, __s4@0001000000000000, __v3@0000000000101000)
    {
    ctxt t5_1, t5_2, t5_3;
    info.eval->multiply_plain(ss[1], bits["0010000000000000"], t5_1);
    info.eval->multiply_plain(ss[4], bits["0001000000000000"], t5_2);
    info.eval->multiply_plain(vs[3], bits["0000000000101000"], t5_3);
    info.eval->add_many({t5_1, t5_2, t5_3}, ts[5]);
    }
    
    info.eval->add(ts[4], ts[5], vs[4]); // __v4 = __t4 + __t5
    info.eval->rotate_rows(vs[4], -9, gk, ss[7]); // __s7 = __v4 >> 9
    info.eval->add(vs[4], ss[5], vs[5]); // __v5 = __v4 + __s5
    info.eval->rotate_rows(vs[5], -8, gk, ss[8]); // __s8 = __v5 >> 8
    
    // __t6 = blend(__s8@0000000000100000, __v4@0000000000001000)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(ss[8], bits["0000000000100000"], t6_1);
    info.eval->multiply_plain(vs[4], bits["0000000000001000"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    
    // __t7 = blend(__v4@0000000000100000, __s7@0000000000001000)
    {
    ctxt t7_1, t7_2;
    info.eval->multiply_plain(vs[4], bits["0000000000100000"], t7_1);
    info.eval->multiply_plain(ss[7], bits["0000000000001000"], t7_2);
    info.eval->add(t7_1, t7_2, ts[7]);
    }
    
    info.eval->add(ts[6], ts[7], vs[6]); // __v6 = __t6 + __t7
    info.eval->rotate_rows(vs[6], -2, gk, ss[9]); // __s9 = __v6 >> 2
    info.eval->add(ss[9], vs[6], vs[7]); // __v7 = __s9 + __v6
    return vs[7];
}
    