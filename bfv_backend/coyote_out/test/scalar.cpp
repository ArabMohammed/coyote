
# include <scalar.hpp>
int ScalarProgram::num_registers()
{
    return 11;
}

std::vector<std::string> ScalarProgram::vars_used()
{
    return {"test(c1):0", "test(c5):0", "test(c2):0", "test(c3):0", "test(c4):0", "test(c6):0"};
}

std::vector<ctxt> ScalarProgram::computation(std::map<std::string, ctxt> locs, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    ctxt regs[num_registers()];
    regs[0] = locs["test(c1):0"];
    regs[1] = locs["test(c2):0"];
    info.eval->add(regs[0], regs[1], regs[2]);
    regs[3] = locs["test(c3):0"];
    info.eval->add(regs[2], regs[3], regs[4]);
    regs[5] = locs["test(c4):0"];
    regs[6] = locs["test(c5):0"];
    regs[7] = locs["test(c6):0"];
    info.eval->multiply(regs[6], regs[7], regs[8]);
    info.eval->relinearize_inplace(regs[8], rk);
    info.eval->multiply(regs[5], regs[8], regs[9]);
    info.eval->relinearize_inplace(regs[9], rk);
    info.eval->multiply(regs[4], regs[9], regs[10]);
    info.eval->relinearize_inplace(regs[10], rk);
    std::vector<ctxt> answer;
    answer.push_back(regs[10]);
    return answer;
}
    