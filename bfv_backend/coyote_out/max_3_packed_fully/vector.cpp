
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(4);
    ts[0] = encrypt_input("1111111111111111111111111111111111111111111111101111111111111111111111110", info);
    ts[1] = encrypt_input("111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111110", info);
    ts[2] = encrypt_input("1010", info);
    ts[3] = encrypt_input("0100", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[10];
    ctxt ss[3];

    vs[0] = ts[0];
    info.eval->rotate_rows(vs[0], -1, gk, ss[0]); // __s0 = __v0 >> 1
    vs[1] = ts[1];
    info.eval->multiply(ss[0], vs[1], vs[2]); // __v2 = __s0 * __v1
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->add(ts[2], vs[0], vs[3]); // __v3 = __t2 + __v0
    info.eval->multiply(vs[3], vs[1], vs[4]); // __v4 = __v3 * __v1
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->rotate_rows(vs[4], -1, gk, ss[1]); // __s1 = __v4 >> 1
    info.eval->add(vs[2], ss[1], vs[5]); // __v5 = __v2 + __s1
    info.eval->rotate_rows(vs[5], -2, gk, ss[2]); // __s2 = __v5 >> 2
    info.eval->add(ts[3], vs[0], vs[6]); // __v6 = __t3 + __v0
    info.eval->multiply(vs[6], vs[5], vs[7]); // __v7 = __v6 * __v5
    info.eval->relinearize_inplace(vs[7], rk);
    info.eval->multiply(vs[0], ss[2], vs[8]); // __v8 = __v0 * __s2
    info.eval->relinearize_inplace(vs[8], rk);
    info.eval->add(vs[8], vs[7], vs[9]); // __v9 = __v8 + __v7
    return vs[9];
}
    