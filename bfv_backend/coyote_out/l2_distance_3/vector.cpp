
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(2);
    ts[0] = encrypt_input("111111111111111111111111111111111111111111111111111110", info);
    ts[1] = encrypt_input("111111111111111111111111111111111111111111111111111110", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[7];
    ctxt ss[2];

    vs[0] = ts[0];
    vs[1] = ts[1];
    info.eval->sub(vs[0], vs[1], vs[2]); // __v2 = __v0 - __v1
    info.eval->sub(vs[0], vs[1], vs[3]); // __v3 = __v0 - __v1
    info.eval->multiply(vs[2], vs[3], vs[4]); // __v4 = __v2 * __v3
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->rotate_rows(vs[4], -2, gk, ss[0]); // __s0 = __v4 >> 2
    info.eval->rotate_rows(vs[4], -1, gk, ss[1]); // __s1 = __v4 >> 1
    info.eval->add(vs[4], ss[0], vs[5]); // __v5 = __v4 + __s0
    info.eval->add(ss[1], vs[5], vs[6]); // __v6 = __s1 + __v5
    return vs[6];
}
    