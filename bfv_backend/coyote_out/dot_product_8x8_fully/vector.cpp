
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "00000001", info);
    add_bitstring(bits, "00001011", info);
    add_bitstring(bits, "00000010", info);
    add_bitstring(bits, "00000100", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(6);
    ts[0] = encrypt_input("111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111", info);
    ts[1] = encrypt_input("111111111111111111111111111111111111111111111111111111111111111111111111111111110111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[6];
    ctxt ss[3];

    vs[0] = ts[0];
    vs[1] = ts[1];
    info.eval->multiply(vs[0], vs[1], vs[2]); // __v2 = __v0 * __v1
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -4, gk, ss[0]); // __s0 = __v2 >> 4
    
    // __t2 = blend(__s0@00001011, __v2@00000100)
    {
    ctxt t2_1, t2_2;
    info.eval->multiply_plain(ss[0], bits["00001011"], t2_1);
    info.eval->multiply_plain(vs[2], bits["00000100"], t2_2);
    info.eval->add(t2_1, t2_2, ts[2]);
    }
    
    
    // __t3 = blend(__v2@00001011, __s0@00000100)
    {
    ctxt t3_1, t3_2;
    info.eval->multiply_plain(vs[2], bits["00001011"], t3_1);
    info.eval->multiply_plain(ss[0], bits["00000100"], t3_2);
    info.eval->add(t3_1, t3_2, ts[3]);
    }
    
    info.eval->add(ts[2], ts[3], vs[3]); // __v3 = __t2 + __t3
    info.eval->rotate_rows(vs[3], -2, gk, ss[1]); // __s1 = __v3 >> 2
    
    // __t4 = blend(__v3@00000010, __s1@00000001)
    {
    ctxt t4_1, t4_2;
    info.eval->multiply_plain(vs[3], bits["00000010"], t4_1);
    info.eval->multiply_plain(ss[1], bits["00000001"], t4_2);
    info.eval->add(t4_1, t4_2, ts[4]);
    }
    
    
    // __t5 = blend(__s1@00000010, __v3@00000001)
    {
    ctxt t5_1, t5_2;
    info.eval->multiply_plain(ss[1], bits["00000010"], t5_1);
    info.eval->multiply_plain(vs[3], bits["00000001"], t5_2);
    info.eval->add(t5_1, t5_2, ts[5]);
    }
    
    info.eval->add(ts[4], ts[5], vs[4]); // __v4 = __t4 + __t5
    info.eval->rotate_rows(vs[4], -7, gk, ss[2]); // __s2 = __v4 >> 7
    info.eval->add(vs[4], ss[2], vs[5]); // __v5 = __v4 + __s2
    return vs[5];
}
    