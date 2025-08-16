
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "000000010000000", info);
    add_bitstring(bits, "000101000000000", info);
    add_bitstring(bits, "010000100000000", info);
    add_bitstring(bits, "100000000000000", info);
    add_bitstring(bits, "000000000100000", info);
    add_bitstring(bits, "000000010100000", info);
    add_bitstring(bits, "110101110001000", info);
    add_bitstring(bits, "110101110101000", info);
    add_bitstring(bits, "000000001010111", info);
    add_bitstring(bits, "110000100000000", info);
    add_bitstring(bits, "000000100000000", info);
    add_bitstring(bits, "000000110100000", info);
    add_bitstring(bits, "000000000001000", info);
    add_bitstring(bits, "000000000101000", info);
    add_bitstring(bits, "000000001110111", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(10);
    ts[0] = encrypt_input("1111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111111111000111111111111111110000", info);
    ts[1] = encrypt_input("1111111111111111111111111111111111110111111111111111111011111111111111111111111111111111111111111111111111111101111111111111111110111111111111111110000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[11];
    ctxt ss[7];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -7, gk, ss[0]); // __s0 = __v0 >> 7
    vs[1] = ts[1];
    info.eval->rotate_rows(vs[1], -7, gk, ss[1]); // __s1 = __v1 >> 7
    
    // __t2 = blend(__v0@110101110001000, __s0@000000001110111)
    {
    ctxt t2_1, t2_2;
    info.eval->multiply_plain(vs[0], bits["110101110001000"], t2_1);
    info.eval->multiply_plain(ss[0], bits["000000001110111"], t2_2);
    info.eval->add(t2_1, t2_2, ts[2]);
    }
    
    
    // __t3 = blend(__v1@110101110101000, __s1@000000001010111)
    {
    ctxt t3_1, t3_2;
    info.eval->multiply_plain(vs[1], bits["110101110101000"], t3_1);
    info.eval->multiply_plain(ss[1], bits["000000001010111"], t3_2);
    info.eval->add(t3_1, t3_2, ts[3]);
    }
    
    info.eval->sub(ts[2], ts[3], vs[2]); // __v2 = __t2 - __t3
    info.eval->rotate_rows(vs[2], -8, gk, ss[2]); // __s2 = __v2 >> 8
    info.eval->sub(vs[0], vs[1], vs[3]); // __v3 = __v0 - __v1
    
    // __t4 = blend(__v3@100000000000000, __s2@010000100000000, __v2@000101000000000)
    {
    ctxt t4_1, t4_2, t4_3;
    info.eval->multiply_plain(vs[3], bits["100000000000000"], t4_1);
    info.eval->multiply_plain(ss[2], bits["010000100000000"], t4_2);
    info.eval->multiply_plain(vs[2], bits["000101000000000"], t4_3);
    info.eval->add_many({t4_1, t4_2, t4_3}, ts[4]);
    }
    
    
    // __t5 = blend(__v2@110000100000000, __s2@000101000000000)
    {
    ctxt t5_1, t5_2;
    info.eval->multiply_plain(vs[2], bits["110000100000000"], t5_1);
    info.eval->multiply_plain(ss[2], bits["000101000000000"], t5_2);
    info.eval->add(t5_1, t5_2, ts[5]);
    }
    
    info.eval->multiply(ts[4], ts[5], vs[4]); // __v4 = __t4 * __t5
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->rotate_rows(vs[4], -6, gk, ss[3]); // __s3 = __v4 >> 6
    
    // __t6 = blend(__s0@000000000100000, __v0@000000000001000)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(ss[0], bits["000000000100000"], t6_1);
    info.eval->multiply_plain(vs[0], bits["000000000001000"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    info.eval->sub(ts[6], vs[1], vs[5]); // __v5 = __t6 - __v1
    
    // __t7 = blend(__s2@000000010000000, __v5@000000000101000)
    {
    ctxt t7_1, t7_2;
    info.eval->multiply_plain(ss[2], bits["000000010000000"], t7_1);
    info.eval->multiply_plain(vs[5], bits["000000000101000"], t7_2);
    info.eval->add(t7_1, t7_2, ts[7]);
    }
    
    info.eval->multiply(vs[2], ts[7], vs[6]); // __v6 = __v2 * __t7
    info.eval->relinearize_inplace(vs[6], rk);
    
    // __t8 = blend(__s3@000000110100000, __v6@000000000001000)
    {
    ctxt t8_1, t8_2;
    info.eval->multiply_plain(ss[3], bits["000000110100000"], t8_1);
    info.eval->multiply_plain(vs[6], bits["000000000001000"], t8_2);
    info.eval->add(t8_1, t8_2, ts[8]);
    }
    
    
    // __t9 = blend(__v4@000000100000000, __v6@000000010100000, __s3@000000000001000)
    {
    ctxt t9_1, t9_2, t9_3;
    info.eval->multiply_plain(vs[4], bits["000000100000000"], t9_1);
    info.eval->multiply_plain(vs[6], bits["000000010100000"], t9_2);
    info.eval->multiply_plain(ss[3], bits["000000000001000"], t9_3);
    info.eval->add_many({t9_1, t9_2, t9_3}, ts[9]);
    }
    
    info.eval->add(ts[8], ts[9], vs[7]); // __v7 = __t8 + __t9
    info.eval->rotate_rows(vs[7], -11, gk, ss[4]); // __s4 = __v7 >> 11
    info.eval->rotate_rows(vs[7], -8, gk, ss[5]); // __s5 = __v7 >> 8
    info.eval->add(ss[4], vs[7], vs[8]); // __v8 = __s4 + __v7
    info.eval->rotate_rows(vs[8], -10, gk, ss[6]); // __s6 = __v8 >> 10
    info.eval->add(ss[4], ss[5], vs[9]); // __v9 = __s4 + __s5
    info.eval->add(ss[6], vs[9], vs[10]); // __v10 = __s6 + __v9
    return vs[10];
}
    