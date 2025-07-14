
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(4);
    ts[0] = encrypt_input("111111111111110111001111111111111101110001111111111111101100111111111111110111", info);
    ts[1] = encrypt_input("111111111111111111001111111111111111110001111111111111111100111111111111111111", info);
    ts[2] = encrypt_input("111111111111111111001111111111111111110001111111111111111100111111111111111111", info);
    ts[3] = encrypt_input("111111111111111111001111111111111111110001111111111111111100111111111111111111", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[7];
    ctxt ss[0];

    vs[0] = ts[0];
    vs[1] = ts[1];
    vs[2] = ts[2];
    vs[3] = ts[3];
    info.eval->multiply(vs[2], vs[0], vs[4]); // __v4 = __v2 * __v0
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->sub(vs[1], vs[4], vs[5]); // __v5 = __v1 - __v4
    info.eval->sub(vs[5], vs[3], vs[6]); // __v6 = __v5 - __v3
    return vs[6];
}
    