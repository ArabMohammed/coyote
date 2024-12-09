
# include <scalar.hpp>
int ScalarProgram::num_registers()
{
    return 63;
}

std::vector<std::string> ScalarProgram::vars_used()
{
    return {"dot_product_16x16_fully(v2):1", "dot_product_16x16_fully(v2):13", "dot_product_16x16_fully(v2):10", "dot_product_16x16_fully(v1):8", "dot_product_16x16_fully(v2):14", "dot_product_16x16_fully(v1):2", "dot_product_16x16_fully(v1):1", "dot_product_16x16_fully(v1):6", "dot_product_16x16_fully(v2):15", "dot_product_16x16_fully(v1):13", "dot_product_16x16_fully(v1):14", "dot_product_16x16_fully(v2):6", "dot_product_16x16_fully(v2):9", "dot_product_16x16_fully(v1):3", "dot_product_16x16_fully(v1):10", "dot_product_16x16_fully(v1):11", "dot_product_16x16_fully(v2):5", "dot_product_16x16_fully(v1):15", "dot_product_16x16_fully(v2):2", "dot_product_16x16_fully(v1):9", "dot_product_16x16_fully(v2):3", "dot_product_16x16_fully(v1):12", "dot_product_16x16_fully(v2):8", "dot_product_16x16_fully(v2):4", "dot_product_16x16_fully(v1):7", "dot_product_16x16_fully(v2):0", "dot_product_16x16_fully(v2):7", "dot_product_16x16_fully(v1):5", "dot_product_16x16_fully(v2):11", "dot_product_16x16_fully(v2):12", "dot_product_16x16_fully(v1):4", "dot_product_16x16_fully(v1):0"};
}

std::vector<ctxt> ScalarProgram::computation(std::map<std::string, ctxt> locs, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    ctxt regs[num_registers()];
    regs[0] = locs["dot_product_16x16_fully(v1):0"];
    regs[1] = locs["dot_product_16x16_fully(v2):0"];
    info.eval->multiply(regs[0], regs[1], regs[2]);
    info.eval->relinearize_inplace(regs[2], rk);
    regs[3] = locs["dot_product_16x16_fully(v1):1"];
    regs[4] = locs["dot_product_16x16_fully(v2):1"];
    info.eval->multiply(regs[3], regs[4], regs[5]);
    info.eval->relinearize_inplace(regs[5], rk);
    info.eval->add(regs[2], regs[5], regs[6]);
    regs[7] = locs["dot_product_16x16_fully(v1):2"];
    regs[8] = locs["dot_product_16x16_fully(v2):2"];
    info.eval->multiply(regs[7], regs[8], regs[9]);
    info.eval->relinearize_inplace(regs[9], rk);
    regs[10] = locs["dot_product_16x16_fully(v1):3"];
    regs[11] = locs["dot_product_16x16_fully(v2):3"];
    info.eval->multiply(regs[10], regs[11], regs[12]);
    info.eval->relinearize_inplace(regs[12], rk);
    info.eval->add(regs[9], regs[12], regs[13]);
    info.eval->add(regs[6], regs[13], regs[14]);
    regs[15] = locs["dot_product_16x16_fully(v1):4"];
    regs[16] = locs["dot_product_16x16_fully(v2):4"];
    info.eval->multiply(regs[15], regs[16], regs[17]);
    info.eval->relinearize_inplace(regs[17], rk);
    regs[18] = locs["dot_product_16x16_fully(v1):5"];
    regs[19] = locs["dot_product_16x16_fully(v2):5"];
    info.eval->multiply(regs[18], regs[19], regs[20]);
    info.eval->relinearize_inplace(regs[20], rk);
    info.eval->add(regs[17], regs[20], regs[21]);
    regs[22] = locs["dot_product_16x16_fully(v1):6"];
    regs[23] = locs["dot_product_16x16_fully(v2):6"];
    info.eval->multiply(regs[22], regs[23], regs[24]);
    info.eval->relinearize_inplace(regs[24], rk);
    regs[25] = locs["dot_product_16x16_fully(v1):7"];
    regs[26] = locs["dot_product_16x16_fully(v2):7"];
    info.eval->multiply(regs[25], regs[26], regs[27]);
    info.eval->relinearize_inplace(regs[27], rk);
    info.eval->add(regs[24], regs[27], regs[28]);
    info.eval->add(regs[21], regs[28], regs[29]);
    info.eval->add(regs[14], regs[29], regs[30]);
    regs[31] = locs["dot_product_16x16_fully(v1):8"];
    regs[32] = locs["dot_product_16x16_fully(v2):8"];
    info.eval->multiply(regs[31], regs[32], regs[33]);
    info.eval->relinearize_inplace(regs[33], rk);
    regs[34] = locs["dot_product_16x16_fully(v1):9"];
    regs[35] = locs["dot_product_16x16_fully(v2):9"];
    info.eval->multiply(regs[34], regs[35], regs[36]);
    info.eval->relinearize_inplace(regs[36], rk);
    info.eval->add(regs[33], regs[36], regs[37]);
    regs[38] = locs["dot_product_16x16_fully(v1):10"];
    regs[39] = locs["dot_product_16x16_fully(v2):10"];
    info.eval->multiply(regs[38], regs[39], regs[40]);
    info.eval->relinearize_inplace(regs[40], rk);
    regs[41] = locs["dot_product_16x16_fully(v1):11"];
    regs[42] = locs["dot_product_16x16_fully(v2):11"];
    info.eval->multiply(regs[41], regs[42], regs[43]);
    info.eval->relinearize_inplace(regs[43], rk);
    info.eval->add(regs[40], regs[43], regs[44]);
    info.eval->add(regs[37], regs[44], regs[45]);
    regs[46] = locs["dot_product_16x16_fully(v1):12"];
    regs[47] = locs["dot_product_16x16_fully(v2):12"];
    info.eval->multiply(regs[46], regs[47], regs[48]);
    info.eval->relinearize_inplace(regs[48], rk);
    regs[49] = locs["dot_product_16x16_fully(v1):13"];
    regs[50] = locs["dot_product_16x16_fully(v2):13"];
    info.eval->multiply(regs[49], regs[50], regs[51]);
    info.eval->relinearize_inplace(regs[51], rk);
    info.eval->add(regs[48], regs[51], regs[52]);
    regs[53] = locs["dot_product_16x16_fully(v1):14"];
    regs[54] = locs["dot_product_16x16_fully(v2):14"];
    info.eval->multiply(regs[53], regs[54], regs[55]);
    info.eval->relinearize_inplace(regs[55], rk);
    regs[56] = locs["dot_product_16x16_fully(v1):15"];
    regs[57] = locs["dot_product_16x16_fully(v2):15"];
    info.eval->multiply(regs[56], regs[57], regs[58]);
    info.eval->relinearize_inplace(regs[58], rk);
    info.eval->add(regs[55], regs[58], regs[59]);
    info.eval->add(regs[52], regs[59], regs[60]);
    info.eval->add(regs[45], regs[60], regs[61]);
    info.eval->add(regs[30], regs[61], regs[62]);
    std::vector<ctxt> answer;
    answer.push_back(regs[62]);
    return answer;
}
    