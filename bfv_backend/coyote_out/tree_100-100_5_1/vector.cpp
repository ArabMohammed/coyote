
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "100000000100000", info);
    add_bitstring(bits, "100000000000000", info);
    add_bitstring(bits, "010001000110010", info);
    add_bitstring(bits, "000010000000000", info);
    add_bitstring(bits, "000010000000010", info);
    add_bitstring(bits, "100010000000001", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(10);
    ts[0] = encrypt_input("11111111110011111111110001111111111001110111111", info);
    ts[1] = encrypt_input("11111111110011110111100001111011111001110111111", info);
    ts[2] = encrypt_input("11111111110011111111110001111111111001110111111", info);
    ts[3] = encrypt_input("11111111110011111111110001111111110001110111111", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[6];
    ctxt ss[3];

    info.eval->multiply(ts[0], ts[1], vs[0]); // __v0 = __t0 * __t1
    info.eval->relinearize_inplace(vs[0], rk);
    info.eval->multiply(ts[2], ts[3], vs[1]); // __v1 = __t2 * __t3
    info.eval->relinearize_inplace(vs[1], rk);
    
    // __t4 = blend(__v0@100010000000001, __v1@010001000110010)
    {
    ctxt t4_1, t4_2;
    info.eval->multiply_plain(vs[0], bits["100010000000001"], t4_1);
    info.eval->multiply_plain(vs[1], bits["010001000110010"], t4_2);
    info.eval->add(t4_1, t4_2, ts[4]);
    }
    
    
    // __t5 = blend(__v1@100010000000001, __v0@010001000110010)
    {
    ctxt t5_1, t5_2;
    info.eval->multiply_plain(vs[1], bits["100010000000001"], t5_1);
    info.eval->multiply_plain(vs[0], bits["010001000110010"], t5_2);
    info.eval->add(t5_1, t5_2, ts[5]);
    }
    
    info.eval->multiply(ts[4], ts[5], vs[2]); // __v2 = __t4 * __t5
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -14, gk, ss[0]); // __s0 = __v2 >> 14
    
    // __t6 = blend(__s0@100000000100000, __v2@000010000000010)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(ss[0], bits["100000000100000"], t6_1);
    info.eval->multiply_plain(vs[2], bits["000010000000010"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    
    // __t7 = blend(__v2@100000000100000, __s0@000010000000010)
    {
    ctxt t7_1, t7_2;
    info.eval->multiply_plain(vs[2], bits["100000000100000"], t7_1);
    info.eval->multiply_plain(ss[0], bits["000010000000010"], t7_2);
    info.eval->add(t7_1, t7_2, ts[7]);
    }
    
    info.eval->multiply(ts[6], ts[7], vs[3]); // __v3 = __t6 * __t7
    info.eval->relinearize_inplace(vs[3], rk);
    info.eval->rotate_rows(vs[3], -6, gk, ss[1]); // __s1 = __v3 >> 6
    
    // __t8 = blend(__v3@100000000000000, __s1@000010000000000)
    {
    ctxt t8_1, t8_2;
    info.eval->multiply_plain(vs[3], bits["100000000000000"], t8_1);
    info.eval->multiply_plain(ss[1], bits["000010000000000"], t8_2);
    info.eval->add(t8_1, t8_2, ts[8]);
    }
    
    
    // __t9 = blend(__s1@100000000000000, __v3@000010000000000)
    {
    ctxt t9_1, t9_2;
    info.eval->multiply_plain(ss[1], bits["100000000000000"], t9_1);
    info.eval->multiply_plain(vs[3], bits["000010000000000"], t9_2);
    info.eval->add(t9_1, t9_2, ts[9]);
    }
    
    info.eval->multiply(ts[8], ts[9], vs[4]); // __v4 = __t8 * __t9
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->rotate_rows(vs[4], -4, gk, ss[2]); // __s2 = __v4 >> 4
    info.eval->add(vs[4], ss[2], vs[5]); // __v5 = __v4 + __s2
    return vs[5];
}
    