
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "010000001", info);
    add_bitstring(bits, "000000001", info);
    add_bitstring(bits, "001000001", info);
    add_bitstring(bits, "001000000", info);
    add_bitstring(bits, "001100000", info);
    add_bitstring(bits, "000000010", info);
    add_bitstring(bits, "000000011", info);
    add_bitstring(bits, "000100000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(27);
    ts[0] = encrypt_input("00000000110111", info);
    ts[1] = encrypt_input("00000000110111", info);
    ts[2] = encrypt_input("011011111011111011000110111110111110111", info);
    ts[3] = encrypt_input("011011011011111011100110111110110110111", info);
    ts[4] = encrypt_input("110101110111000000110111", info);
    ts[5] = encrypt_input("11010100000000", info);
    ts[7] = encrypt_input("11011000000000", info);
    ts[8] = encrypt_input("11011100000000", info);
    ts[11] = encrypt_input("0011011100000110110", info);
    ts[12] = encrypt_input("0011011100000110111", info);
    ts[13] = encrypt_input("0011011100001101110", info);
    ts[14] = encrypt_input("00000001101110", info);
    ts[17] = encrypt_input("000110111000110110110111", info);
    ts[21] = encrypt_input("00000001101110", info);
    ts[22] = encrypt_input("00011011100000", info);
    ts[25] = encrypt_input("00011011100000", info);
    ts[26] = encrypt_input("00000000110111", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[15];
    ctxt ss[5];

    info.eval->add(ts[0], ts[1], vs[0]); // __v0 = __t0 + __t1
    info.eval->multiply(ts[2], ts[3], vs[1]); // __v1 = __t2 * __t3
    info.eval->relinearize_inplace(vs[1], rk);
    info.eval->rotate_rows(vs[1], -2, gk, ss[0]); // __s0 = __v1 >> 2
    
    // __t6 = blend(__v1@010000001, __t5@100000000)
    {
    ctxt t6_1;
    info.eval->multiply_plain(vs[1], bits["010000001"], t6_1);
    info.eval->add(t6_1, ts[5], ts[6]);
    }
    
    info.eval->add(ts[4], ts[6], vs[2]); // __v2 = __t4 + __t6
    info.eval->rotate_rows(vs[2], -2, gk, ss[1]); // __s1 = __v2 >> 2
    
    // __t9 = blend(__v2@000000001, __t7@100000000)
    {
    ctxt t9_1;
    info.eval->multiply_plain(vs[2], bits["000000001"], t9_1);
    info.eval->add(t9_1, ts[7], ts[9]);
    }
    
    
    // __t10 = blend(__v0@000000001, __t8@100000000)
    {
    ctxt t10_1;
    info.eval->multiply_plain(vs[0], bits["000000001"], t10_1);
    info.eval->add(t10_1, ts[8], ts[10]);
    }
    
    info.eval->add(ts[9], ts[10], vs[3]); // __v3 = __t9 + __t10
    info.eval->multiply(vs[2], vs[3], vs[4]); // __v4 = __v2 * __v3
    info.eval->relinearize_inplace(vs[4], rk);
    info.eval->rotate_rows(vs[4], -2, gk, ss[2]); // __s2 = __v4 >> 2
    info.eval->add(ts[11], ts[12], vs[5]); // __v5 = __t11 + __t12
    
    // __t15 = blend(__s0@000000001, __t13@001000010)
    {
    ctxt t15_1;
    info.eval->multiply_plain(ss[0], bits["000000001"], t15_1);
    info.eval->add(t15_1, ts[13], ts[15]);
    }
    
    
    // __t16 = blend(__v5@001000001, __t14@000000010)
    {
    ctxt t16_1;
    info.eval->multiply_plain(vs[5], bits["001000001"], t16_1);
    info.eval->add(t16_1, ts[14], ts[16]);
    }
    
    info.eval->add(ts[15], ts[16], vs[6]); // __v6 = __t15 + __t16
    
    // __t18 = blend(__v6@001000000, __t17@000100011)
    {
    ctxt t18_1;
    info.eval->multiply_plain(vs[6], bits["001000000"], t18_1);
    info.eval->add(t18_1, ts[17], ts[18]);
    }
    
    
    // __t19 = blend(__v1@001100000, __v6@000000011)
    {
    ctxt t19_1, t19_2;
    info.eval->multiply_plain(vs[1], bits["001100000"], t19_1);
    info.eval->multiply_plain(vs[6], bits["000000011"], t19_2);
    info.eval->add(t19_1, t19_2, ts[19]);
    }
    
    info.eval->multiply(ts[18], ts[19], vs[7]); // __v7 = __t18 * __t19
    info.eval->relinearize_inplace(vs[7], rk);
    
    // __t20 = blend(__s1@000100000, __v1@000000010)
    {
    ctxt t20_1, t20_2;
    info.eval->multiply_plain(ss[1], bits["000100000"], t20_1);
    info.eval->multiply_plain(vs[1], bits["000000010"], t20_2);
    info.eval->add(t20_1, t20_2, ts[20]);
    }
    
    info.eval->add(vs[7], ts[20], vs[8]); // __v8 = __v7 + __t20
    
    // __t23 = blend(__s2@001000000, __v8@000100000, __v3@000000001, __t21@000000010)
    {
    ctxt t23_1, t23_2, t23_3;
    info.eval->multiply_plain(ss[2], bits["001000000"], t23_1);
    info.eval->multiply_plain(vs[8], bits["000100000"], t23_2);
    info.eval->multiply_plain(vs[3], bits["000000001"], t23_3);
    info.eval->add_many({t23_1, t23_2, t23_3, ts[21]}, ts[23]);
    }
    
    
    // __t24 = blend(__v7@001000001, __v8@000000010, __t22@000100000)
    {
    ctxt t24_1, t24_2;
    info.eval->multiply_plain(vs[7], bits["001000001"], t24_1);
    info.eval->multiply_plain(vs[8], bits["000000010"], t24_2);
    info.eval->add_many({t24_1, t24_2, ts[22]}, ts[24]);
    }
    
    info.eval->multiply(ts[23], ts[24], vs[9]); // __v9 = __t23 * __t24
    info.eval->relinearize_inplace(vs[9], rk);
    info.eval->rotate_rows(vs[9], -1, gk, ss[3]); // __s3 = __v9 >> 1
    info.eval->add(ts[25], vs[9], vs[10]); // __v10 = __t25 + __v9
    info.eval->add(vs[9], ts[26], vs[11]); // __v11 = __v9 + __t26
    info.eval->multiply(vs[11], ss[3], vs[12]); // __v12 = __v11 * __s3
    info.eval->relinearize_inplace(vs[12], rk);
    info.eval->rotate_rows(vs[12], -4, gk, ss[4]); // __s4 = __v12 >> 4
    info.eval->multiply(ss[3], ss[4], vs[13]); // __v13 = __s3 * __s4
    info.eval->relinearize_inplace(vs[13], rk);
    info.eval->add(vs[13], vs[10], vs[14]); // __v14 = __v13 + __v10
    return vs[14];
}
    