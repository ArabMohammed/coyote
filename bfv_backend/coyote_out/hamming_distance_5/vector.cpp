
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(3);
    ts[0] = encrypt_input("1111111111111111111111100111111111111111111111111111111111111111111111101111111111111111111111111111111111111111111110", info);
    ts[1] = encrypt_input("1111111111111111111111100111111111111111111111111111111111111111111111101111111111111111111111111111111111111111111110", info);
    ts[2] = encrypt_input("10011011", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[8];
    ctxt ss[0];

    vs[0] = ts[0];
    vs[1] = ts[1];
    info.eval->multiply(vs[0], vs[1], vs[2]); // __v2 = __v0 * __v1
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->add(vs[0], vs[1], vs[3]); // __v3 = __v0 + __v1
    info.eval->multiply(ts[2], vs[2], vs[4]); // __v4 = __t2 * __v2
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->sub(vs[3], vs[4], vs[5]); // __v5 = __v3 - __v4
    info.eval->add(vs[0], vs[1], vs[6]); // __v6 = __v0 + __v1
    info.eval->sub(vs[6], vs[4], vs[7]); // __v7 = __v6 - __v4
    return vs[7];
}
    