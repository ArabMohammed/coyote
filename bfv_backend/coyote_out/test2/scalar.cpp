
# include <scalar.hpp>
int ScalarProgram::num_registers()
{
    return 15;
}

std::vector<std::string> ScalarProgram::vars_used()
{
    return {"test2(c2):0", "test2(c5):0", "test2(c1):0", "test2(c3):0", "test2(c7):0", "test2(c4):0", "test2(c8):0", "test2(c6):0"};
}

std::vector<ctxt> ScalarProgram::computation(std::map<std::string, ctxt> locs, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    ctxt regs[num_registers()];
    regs[0] = locs["test2(c1):0"];
    regs[1] = locs["test2(c2):0"];
    info.eval->add(regs[0], regs[1], regs[2]);
    regs[3] = locs["test2(c3):0"];
    regs[4] = locs["test2(c4):0"];
    info.eval->add(regs[3], regs[4], regs[5]);
    info.eval->multiply(regs[2], regs[5], regs[6]);
    info.eval->relinearize_inplace(regs[6], rk);
    regs[7] = locs["test2(c5):0"];
    regs[8] = locs["test2(c6):0"];
    info.eval->add(regs[7], regs[8], regs[9]);
    regs[10] = locs["test2(c7):0"];
    regs[11] = locs["test2(c8):0"];
    info.eval->add(regs[10], regs[11], regs[12]);
    info.eval->multiply(regs[9], regs[12], regs[13]);
    info.eval->relinearize_inplace(regs[13], rk);
    info.eval->add(regs[6], regs[13], regs[14]);
    std::vector<ctxt> answer;
    answer.push_back(regs[14]);
    return answer;
}
    