
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "0001000100000010", info);
    add_bitstring(bits, "0000000001000001", info);
    add_bitstring(bits, "0000010001000101", info);
    add_bitstring(bits, "0100000000000000", info);
    add_bitstring(bits, "0000010000000100", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(7);
    ts[0] = encrypt_input("01111111111111101110111111111111110110011111111111111011101111111111111101110111111111111110111000111111111111110111111111111111110111111111111111110111", info);
    ts[1] = encrypt_input("11111111111111111100111111111111111110111111111111111111001111111111111111110111111111111111111001111111111111111110111111111111111111111111111111111111", info);
    ts[2] = encrypt_input("01111111111111111110111111111111111110011111111111111111101111111111111111110111111111111111111000111111111111111111111111111111111111111111111111111111", info);
    ts[3] = encrypt_input("01111111111111111110111111111111111110011111111111111111101111111111111111110111111111111111111000111111111111111111111111111111111111111111111111111111", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[10];
    ctxt ss[1];

    vs[0] = ts[0];
    vs[1] = ts[1];
    info.eval->rotate_rows(vs[1], -1, gk, ss[0]); // __s0 = __v1 >> 1
    vs[2] = ts[2];
    vs[3] = ts[3];
    info.eval->multiply(vs[2], vs[0], vs[4]); // __v4 = __v2 * __v0
    info.eval->relinearize_inplace(vs[4], rk);
    
    // __t4 = blend(__s0@0100000000000000, __v1@0001000100000010)
    {
    ctxt t4_1, t4_2;
    info.eval->multiply_plain(ss[0], bits["0100000000000000"], t4_1);
    info.eval->multiply_plain(vs[1], bits["0001000100000010"], t4_2);
    info.eval->add(t4_1, t4_2, ts[4]);
    }
    
    info.eval->sub(ts[4], vs[4], vs[5]); // __v5 = __t4 - __v4
    info.eval->sub(vs[5], vs[3], vs[6]); // __v6 = __v5 - __v3
    
    // __t5 = blend(__v5@0100000000000000, __s0@0000010000000100, __v1@0000000001000001)
    {
    ctxt t5_1, t5_2, t5_3;
    info.eval->multiply_plain(vs[5], bits["0100000000000000"], t5_1);
    info.eval->multiply_plain(ss[0], bits["0000010000000100"], t5_2);
    info.eval->multiply_plain(vs[1], bits["0000000001000001"], t5_3);
    info.eval->add_many({t5_1, t5_2, t5_3}, ts[5]);
    }
    
    
    // __t6 = blend(__v3@0100000000000000, __v4@0000010001000101)
    {
    ctxt t6_1, t6_2;
    info.eval->multiply_plain(vs[3], bits["0100000000000000"], t6_1);
    info.eval->multiply_plain(vs[4], bits["0000010001000101"], t6_2);
    info.eval->add(t6_1, t6_2, ts[6]);
    }
    
    info.eval->sub(ts[5], ts[6], vs[7]); // __v7 = __t5 - __t6
    info.eval->sub(vs[7], vs[3], vs[8]); // __v8 = __v7 - __v3
    info.eval->sub(vs[7], vs[3], vs[9]); // __v9 = __v7 - __v3
    return vs[9];
}
    