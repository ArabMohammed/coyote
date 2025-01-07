
# include <scalar.hpp>
int ScalarProgram::num_registers()
{
    return 24;
}

std::vector<std::string> ScalarProgram::vars_used()
{
    return {"hamming_distance_4(a):3", "hamming_distance_4(a):0", "hamming_distance_4(b):3", "hamming_distance_4(b):2", "2", "hamming_distance_4(b):0", "hamming_distance_4(a):1", "hamming_distance_4(b):1", "hamming_distance_4(a):2"};
}

std::vector<ctxt> ScalarProgram::computation(std::map<std::string, ctxt> locs, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    ctxt regs[num_registers()];
    regs[0] = locs["hamming_distance_4(a):0"];
    regs[1] = locs["hamming_distance_4(b):0"];
    info.eval->add(regs[0], regs[1], regs[2]);
    info.eval->multiply(regs[0], regs[1], regs[3]);
    info.eval->relinearize_inplace(regs[3], rk);
    info.eval->multiply(locs["2"], regs[3], regs[4]);
    info.eval->relinearize_inplace(regs[4], rk);
    info.eval->sub(regs[2], regs[4], regs[5]);
    regs[6] = locs["hamming_distance_4(a):1"];
    regs[7] = locs["hamming_distance_4(b):1"];
    info.eval->add(regs[6], regs[7], regs[8]);
    info.eval->multiply(regs[6], regs[7], regs[9]);
    info.eval->relinearize_inplace(regs[9], rk);
    info.eval->multiply(locs["2"], regs[9], regs[10]);
    info.eval->relinearize_inplace(regs[10], rk);
    info.eval->sub(regs[8], regs[10], regs[11]);
    regs[12] = locs["hamming_distance_4(a):2"];
    regs[13] = locs["hamming_distance_4(b):2"];
    info.eval->add(regs[12], regs[13], regs[14]);
    info.eval->multiply(regs[12], regs[13], regs[15]);
    info.eval->relinearize_inplace(regs[15], rk);
    info.eval->multiply(locs["2"], regs[15], regs[16]);
    info.eval->relinearize_inplace(regs[16], rk);
    info.eval->sub(regs[14], regs[16], regs[17]);
    regs[18] = locs["hamming_distance_4(a):3"];
    regs[19] = locs["hamming_distance_4(b):3"];
    info.eval->add(regs[18], regs[19], regs[20]);
    info.eval->multiply(regs[18], regs[19], regs[21]);
    info.eval->relinearize_inplace(regs[21], rk);
    info.eval->multiply(locs["2"], regs[21], regs[22]);
    info.eval->relinearize_inplace(regs[22], rk);
    info.eval->sub(regs[20], regs[22], regs[23]);
    std::vector<ctxt> answer;
    answer.push_back(regs[5]);
    answer.push_back(regs[11]);
    answer.push_back(regs[17]);
    answer.push_back(regs[23]);
    return answer;
}
    