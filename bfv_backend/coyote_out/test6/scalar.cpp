
# include <scalar.hpp>
int ScalarProgram::num_registers()
{
    return 7;
}

std::vector<std::string> ScalarProgram::vars_used()
{
    return {"test6(c2):0", "test6(c1):0", "test6(c4):0", "test6(c3):0"};
}

std::vector<ctxt> ScalarProgram::computation(std::map<std::string, ctxt> locs, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    ctxt regs[num_registers()];
    regs[0] = locs["test6(c1):0"];
    regs[1] = locs["test6(c2):0"];
    info.eval->multiply(regs[0], regs[1], regs[2]);
    info.eval->relinearize_inplace(regs[2], rk);
    regs[3] = locs["test6(c3):0"];
    regs[4] = locs["test6(c4):0"];
    info.eval->add(regs[3], regs[4], regs[5]);
    info.eval->add(regs[2], regs[5], regs[6]);
    std::vector<ctxt> answer;
    answer.push_back(regs[6]);
    return answer;
}
    