
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "0000000000101", info);
    add_bitstring(bits, "0001010000010", info);
    add_bitstring(bits, "0000000000100", info);
    add_bitstring(bits, "0000100000101", info);
    add_bitstring(bits, "0000000000010", info);
    add_bitstring(bits, "0000000000011", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(10);
    ts[0] = encrypt_input("1111111111110110001111111111110111111111111111011011111111111101111111111111110111011111111111101110111111111111011111111111111101101111111111110111", info);
    ts[1] = encrypt_input("0000000000111111111111111111111111111111101111111111111111", info);
    ts[2] = encrypt_input("0000000000111111111111111111111111111111101111111111111111", info);
    ts[3] = encrypt_input("0001111111111111111011111111111111110000011111111111111100", info);
    ts[4] = encrypt_input("0000111111111111111000000111111111111111101111111111111111", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[12];
    ctxt ss[3];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -4, gk, ss[0]); // __s0 = __v0 >> 4
    vs[1] = ts[1];
    vs[2] = ts[2];
    vs[3] = ts[3];
    vs[4] = ts[4];
    
    // __t5 = blend(__v0@0001010000010, __s0@0000100000101)
    {
    ctxt t5_1, t5_2;
    info.eval->multiply_plain(vs[0], bits["0001010000010"], t5_1);
    info.eval->multiply_plain(ss[0], bits["0000100000101"], t5_2);
    info.eval->add(t5_1, t5_2, ts[5]);
    }
    
    
    // __t6 = blend(__v3@0001010000010, __v0@0000100000101)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(vs[3], bits["0001010000010"], t6_1);
    info.eval->multiply_plain(vs[0], bits["0000100000101"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    info.eval->multiply(ts[5], ts[6], vs[5]); // __v5 = __t5 * __t6
    info.eval->relinearize_inplace(vs[5], rk);
    info.eval->rotate_rows(vs[5], -7, gk, ss[1]); // __s1 = __v5 >> 7
    info.eval->multiply(vs[5], vs[4], vs[6]); // __v6 = __v5 * __v4
    info.eval->relinearize_inplace(vs[6], rk);
    info.eval->rotate_rows(vs[6], -7, gk, ss[2]); // __s2 = __v6 >> 7
    info.eval->multiply(vs[5], vs[4], vs[7]); // __v7 = __v5 * __v4
    info.eval->relinearize_inplace(vs[7], rk);
    
    // __t7 = blend(__v7@0000000000101, __s2@0000000000010)
    {
    ctxt t7_1, t7_2;
    info.eval->multiply_plain(vs[7], bits["0000000000101"], t7_1);
    info.eval->multiply_plain(ss[2], bits["0000000000010"], t7_2);
    info.eval->add(t7_1, t7_2, ts[7]);
    }
    
    
    // __t8 = blend(__s1@0000000000101, __v5@0000000000010)
    {
    ctxt t8_1, t8_2;
    info.eval->multiply_plain(ss[1], bits["0000000000101"], t8_1);
    info.eval->multiply_plain(vs[5], bits["0000000000010"], t8_2);
    info.eval->add(t8_1, t8_2, ts[8]);
    }
    
    info.eval->add(ts[7], ts[8], vs[8]); // __v8 = __t7 + __t8
    info.eval->add(vs[8], vs[2], vs[9]); // __v9 = __v8 + __v2
    info.eval->add(vs[8], vs[2], vs[10]); // __v10 = __v8 + __v2
    
    // __t9 = blend(__v10@0000000000100, __v9@0000000000011)
    {
    ctxt t9_1, t9_2;
    info.eval->multiply_plain(vs[10], bits["0000000000100"], t9_1);
    info.eval->multiply_plain(vs[9], bits["0000000000011"], t9_2);
    info.eval->add(t9_1, t9_2, ts[9]);
    }
    
    info.eval->sub(vs[1], ts[9], vs[11]); // __v11 = __v1 - __t9
    return vs[11];
}
    