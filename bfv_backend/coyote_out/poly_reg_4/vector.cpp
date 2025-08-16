
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "00000000101000", info);
    add_bitstring(bits, "00010001000000", info);
    add_bitstring(bits, "00000001000000", info);
    add_bitstring(bits, "00000000100000", info);
    add_bitstring(bits, "00000010110000", info);
    add_bitstring(bits, "00000000001000", info);
    add_bitstring(bits, "00010010000000", info);
    add_bitstring(bits, "00000000110000", info);
    add_bitstring(bits, "00010000100000", info);
    add_bitstring(bits, "00010000000000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(12);
    ts[0] = encrypt_input("11111111111101110011111111111101111111111111110110111111111111011111111111111101101111111111110111111111111111011111111111111101111111111111110111111111111111011011111111111101111111111111110111", info);
    ts[1] = encrypt_input("00011111111111111110001111111111111110111111111111111101111111111111111000", info);
    ts[2] = encrypt_input("00011111111111111110001111111111111110111111111111111101111111111111111000", info);
    ts[3] = encrypt_input("00011111111111111110011111111111111100111111111111111101111111111111111000", info);
    ts[4] = encrypt_input("00011111111111111110001111111111111110111111111111111101111111111111111000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[13];
    ctxt ss[3];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -3, gk, ss[0]); // __s0 = __v0 >> 3
    info.eval->rotate_rows(vs[0], -10, gk, ss[1]); // __s1 = __v0 >> 10
    vs[1] = ts[1];
    vs[2] = ts[2];
    vs[3] = ts[3];
    vs[4] = ts[4];
    
    // __t5 = blend(__s1@00010000000000, __v0@00000010110000)
    {
    ctxt t5_1, t5_2;
    info.eval->multiply_plain(ss[1], bits["00010000000000"], t5_1);
    info.eval->multiply_plain(vs[0], bits["00000010110000"], t5_2);
    info.eval->add(t5_1, t5_2, ts[5]);
    }
    
    
    // __t6 = blend(__v3@00010010000000, __s1@00000000110000)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(vs[3], bits["00010010000000"], t6_1);
    info.eval->multiply_plain(ss[1], bits["00000000110000"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    info.eval->multiply(ts[5], ts[6], vs[5]); // __v5 = __t5 * __t6
    info.eval->relinearize_inplace(vs[5], rk);
    info.eval->rotate_rows(vs[5], -1, gk, ss[2]); // __s2 = __v5 >> 1
    
    // __t7 = blend(__s0@00010000100000, __s1@00000001000000, __v0@00000000001000)
    {
    ctxt t7_1, t7_2, t7_3;
    info.eval->multiply_plain(ss[0], bits["00010000100000"], t7_1);
    info.eval->multiply_plain(ss[1], bits["00000001000000"], t7_2);
    info.eval->multiply_plain(vs[0], bits["00000000001000"], t7_3);
    info.eval->add_many({t7_1, t7_2, t7_3}, ts[7]);
    }
    
    
    // __t8 = blend(__v0@00010000000000, __s0@00000001000000, __v3@00000000101000)
    {
    ctxt t8_1, t8_2, t8_3;
    info.eval->multiply_plain(vs[0], bits["00010000000000"], t8_1);
    info.eval->multiply_plain(ss[0], bits["00000001000000"], t8_2);
    info.eval->multiply_plain(vs[3], bits["00000000101000"], t8_3);
    info.eval->add_many({t8_1, t8_2, t8_3}, ts[8]);
    }
    
    info.eval->multiply(ts[7], ts[8], vs[6]); // __v6 = __t7 * __t8
    info.eval->relinearize_inplace(vs[6], rk);
    
    // __t9 = blend(__v6@00010001000000, __v5@00000000100000, __s2@00000000001000)
    {
    ctxt t9_1, t9_2, t9_3;
    info.eval->multiply_plain(vs[6], bits["00010001000000"], t9_1);
    info.eval->multiply_plain(vs[5], bits["00000000100000"], t9_2);
    info.eval->multiply_plain(ss[2], bits["00000000001000"], t9_3);
    info.eval->add_many({t9_1, t9_2, t9_3}, ts[9]);
    }
    
    info.eval->multiply(ts[9], vs[4], vs[7]); // __v7 = __t9 * __v4
    info.eval->relinearize_inplace(vs[7], rk);
    
    // __t10 = blend(__v5@00010000000000, __s2@00000001000000, __v6@00000000101000)
    {
    ctxt t10_1, t10_2, t10_3;
    info.eval->multiply_plain(vs[5], bits["00010000000000"], t10_1);
    info.eval->multiply_plain(ss[2], bits["00000001000000"], t10_2);
    info.eval->multiply_plain(vs[6], bits["00000000101000"], t10_3);
    info.eval->add_many({t10_1, t10_2, t10_3}, ts[10]);
    }
    
    info.eval->add(vs[7], ts[10], vs[8]); // __v8 = __v7 + __t10
    info.eval->add(vs[8], vs[2], vs[9]); // __v9 = __v8 + __v2
    info.eval->sub(vs[1], vs[9], vs[10]); // __v10 = __v1 - __v9
    info.eval->add(vs[8], vs[2], vs[11]); // __v11 = __v8 + __v2
    
    // __t11 = blend(__v9@00010000000000, __v11@00000001000000)
    {
    ctxt t11_1, t11_2;
    info.eval->multiply_plain(vs[9], bits["00010000000000"], t11_1);
    info.eval->multiply_plain(vs[11], bits["00000001000000"], t11_2);
    info.eval->add(t11_1, t11_2, ts[11]);
    }
    
    info.eval->sub(vs[1], ts[11], vs[12]); // __v12 = __v1 - __t11
    return vs[12];
}
    