
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(6);
    ts[0] = encrypt_input("1111111110", info);
    ts[1] = encrypt_input("1111111110", info);
    ts[2] = encrypt_input("1111111110", info);
    ts[3] = encrypt_input("1111111110", info);
    ts[4] = encrypt_input("1111111110", info);
    ts[5] = encrypt_input("1111111110", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[11];
    ctxt ss[0];

    vs[0] = ts[0];
    vs[1] = ts[1];
    vs[2] = ts[2];
    vs[3] = ts[3];
    vs[4] = ts[4];
    vs[5] = ts[5];
    info.eval->add(vs[0], vs[1], vs[6]); // __v6 = __v0 + __v1
    info.eval->multiply(vs[4], vs[5], vs[7]); // __v7 = __v4 * __v5
    info.eval->relinearize_inplace(vs[7], rk);
    info.eval->multiply(vs[3], vs[7], vs[8]); // __v8 = __v3 * __v7
    info.eval->relinearize_inplace(vs[8], rk);
    info.eval->add(vs[6], vs[2], vs[9]); // __v9 = __v6 + __v2
    info.eval->multiply(vs[9], vs[8], vs[10]); // __v10 = __v9 * __v8
    info.eval->relinearize_inplace(vs[10], rk);
    return vs[10];
}
    