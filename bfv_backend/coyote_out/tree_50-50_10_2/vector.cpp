
# include <vector.hpp>

std::map<std::string, ptxt> VectorProgram::make_bits(RuntimeContext &info)
{
    std::map<std::string, ptxt> bits;
    add_bitstring(bits, "0010001000", info);
    add_bitstring(bits, "0000000001", info);
    add_bitstring(bits, "0010000000", info);
    add_bitstring(bits, "0000001000", info);
    add_bitstring(bits, "0100000000", info);
    add_bitstring(bits, "0000000100", info);
    add_bitstring(bits, "0000100000", info);
    return bits;
}

std::vector<ctxt> VectorProgram::initialize_temps(RuntimeContext &info)
{
    std::vector<ctxt> ts(30);
    ts[0] = encrypt_input("01101110000011011100", info);
    ts[1] = encrypt_input("01101110000011011100", info);
    ts[2] = encrypt_input("01101110110111110111011011100110111", info);
    ts[3] = encrypt_input("01101100110111110111011011100110111", info);
    ts[4] = encrypt_input("1101110110111001101010000", info);
    ts[5] = encrypt_input("1101110110110001101010000", info);
    ts[8] = encrypt_input("00110111000110110000", info);
    ts[11] = encrypt_input("000011011100000", info);
    ts[12] = encrypt_input("00000011011100110111", info);
    ts[15] = encrypt_input("000000000110111", info);
    ts[16] = encrypt_input("000011011000000", info);
    ts[17] = encrypt_input("001101110000000", info);
    ts[20] = encrypt_input("000011011100000", info);
    ts[24] = encrypt_input("000011011100000", info);
    ts[25] = encrypt_input("000011011100000", info);
    ts[26] = encrypt_input("011011100000000", info);
    ts[29] = encrypt_input("000011011100000", info);
    return ts;
}

ctxt VectorProgram::computation(std::vector<ctxt> ts, std::map<std::string, ptxt> bits, RuntimeContext &info)
{
    seal::RelinKeys rk = info.keys->rk;
    seal::GaloisKeys gk = info.keys->gk;

    ctxt vs[15];
    ctxt ss[7];

    info.eval->multiply(ts[0], ts[1], vs[0]); // __v0 = __t0 * __t1
    info.eval->relinearize_inplace(vs[0], rk);
    info.eval->add(ts[2], ts[3], vs[1]); // __v1 = __t2 + __t3
    info.eval->rotate_rows(vs[1], -1, gk, ss[0]); // __s0 = __v1 >> 1
    
    // __t6 = blend(__v0@0100000000, __t4@1010010000)
    {
    ctxt t6_1;
    info.eval->multiply_plain(vs[0], bits["0100000000"], t6_1);
    info.eval->add(t6_1, ts[4], ts[6]);
    }
    
    
    // __t7 = blend(__v1@0100000000, __t5@1010010000)
    {
    ctxt t7_1;
    info.eval->multiply_plain(vs[1], bits["0100000000"], t7_1);
    info.eval->add(t7_1, ts[5], ts[7]);
    }
    
    info.eval->multiply(ts[6], ts[7], vs[2]); // __v2 = __t6 * __t7
    info.eval->relinearize_inplace(vs[2], rk);
    info.eval->rotate_rows(vs[2], -1, gk, ss[1]); // __s1 = __v2 >> 1
    
    // __t9 = blend(__s1@0010001000, __v0@0000000100)
    {
    ctxt t9_1, t9_2;
    info.eval->multiply_plain(ss[1], bits["0010001000"], t9_1);
    info.eval->multiply_plain(vs[0], bits["0000000100"], t9_2);
    info.eval->add(t9_1, t9_2, ts[9]);
    }
    
    
    // __t10 = blend(__s0@0000000100, __t8@0010001000)
    {
    ctxt t10_1;
    info.eval->multiply_plain(ss[0], bits["0000000100"], t10_1);
    info.eval->add(t10_1, ts[8], ts[10]);
    }
    
    info.eval->multiply(ts[9], ts[10], vs[3]); // __v3 = __t9 * __t10
    info.eval->relinearize_inplace(vs[3], rk);
    info.eval->rotate_rows(vs[3], -5, gk, ss[2]); // __s2 = __v3 >> 5
    
    // __t13 = blend(__v3@0000001000, __v1@0000000001, __t11@0000100000)
    {
    ctxt t13_1, t13_2;
    info.eval->multiply_plain(vs[3], bits["0000001000"], t13_1);
    info.eval->multiply_plain(vs[1], bits["0000000001"], t13_2);
    info.eval->add_many({t13_1, t13_2, ts[11]}, ts[13]);
    }
    
    
    // __t14 = blend(__v1@0000100000, __t12@0000001001)
    {
    ctxt t14_1;
    info.eval->multiply_plain(vs[1], bits["0000100000"], t14_1);
    info.eval->add(t14_1, ts[12], ts[14]);
    }
    
    info.eval->add(ts[13], ts[14], vs[4]); // __v4 = __t13 + __t14
    info.eval->rotate_rows(vs[4], -5, gk, ss[3]); // __s3 = __v4 >> 5
    info.eval->multiply(ts[15], vs[4], vs[5]); // __v5 = __t15 * __v4
    info.eval->relinearize_inplace(vs[5], rk);
    info.eval->rotate_rows(vs[5], -5, gk, ss[4]); // __s4 = __v5 >> 5
    
    // __t18 = blend(__v2@0010000000, __t16@0000100000)
    {
    ctxt t18_1;
    info.eval->multiply_plain(vs[2], bits["0010000000"], t18_1);
    info.eval->add(t18_1, ts[16], ts[18]);
    }
    
    
    // __t19 = blend(__v4@0000100000, __t17@0010000000)
    {
    ctxt t19_1;
    info.eval->multiply_plain(vs[4], bits["0000100000"], t19_1);
    info.eval->add(t19_1, ts[17], ts[19]);
    }
    
    info.eval->add(ts[18], ts[19], vs[6]); // __v6 = __t18 + __t19
    
    // __t21 = blend(__s3@0100000000, __s2@0010000000, __s4@0000100000)
    {
    ctxt t21_1, t21_2, t21_3;
    info.eval->multiply_plain(ss[3], bits["0100000000"], t21_1);
    info.eval->multiply_plain(ss[2], bits["0010000000"], t21_2);
    info.eval->multiply_plain(ss[4], bits["0000100000"], t21_3);
    info.eval->add_many({t21_1, t21_2, t21_3}, ts[21]);
    }
    
    
    // __t22 = blend(__s1@0100000000, __v6@0010000000, __t20@0000100000)
    {
    ctxt t22_1, t22_2;
    info.eval->multiply_plain(ss[1], bits["0100000000"], t22_1);
    info.eval->multiply_plain(vs[6], bits["0010000000"], t22_2);
    info.eval->add_many({t22_1, t22_2, ts[20]}, ts[22]);
    }
    
    info.eval->multiply(ts[21], ts[22], vs[7]); // __v7 = __t21 * __t22
    info.eval->relinearize_inplace(vs[7], rk);
    
    // __t23 = blend(__v3@0010000000, __v6@0000100000)
    {
    ctxt t23_1, t23_2;
    info.eval->multiply_plain(vs[3], bits["0010000000"], t23_1);
    info.eval->multiply_plain(vs[6], bits["0000100000"], t23_2);
    info.eval->add(t23_1, t23_2, ts[23]);
    }
    
    info.eval->add(vs[7], ts[23], vs[8]); // __v8 = __v7 + __t23
    info.eval->rotate_rows(vs[8], -2, gk, ss[5]); // __s5 = __v8 >> 2
    info.eval->add(ss[5], ts[24], vs[9]); // __v9 = __s5 + __t24
    info.eval->add(vs[9], vs[8], vs[10]); // __v10 = __v9 + __v8
    
    // __t27 = blend(__v7@0100000000, __t25@0000100000)
    {
    ctxt t27_1;
    info.eval->multiply_plain(vs[7], bits["0100000000"], t27_1);
    info.eval->add(t27_1, ts[25], ts[27]);
    }
    
    
    // __t28 = blend(__v10@0000100000, __t26@0100000000)
    {
    ctxt t28_1;
    info.eval->multiply_plain(vs[10], bits["0000100000"], t28_1);
    info.eval->add(t28_1, ts[26], ts[28]);
    }
    
    info.eval->multiply(ts[27], ts[28], vs[11]); // __v11 = __t27 * __t28
    info.eval->relinearize_inplace(vs[11], rk);
    info.eval->rotate_rows(vs[11], -3, gk, ss[6]); // __s6 = __v11 >> 3
    info.eval->add(vs[11], ss[0], vs[12]); // __v12 = __v11 + __s0
    info.eval->add(ts[29], vs[12], vs[13]); // __v13 = __t29 + __v12
    info.eval->multiply(ss[6], vs[13], vs[14]); // __v14 = __s6 * __v13
    info.eval->relinearize_inplace(vs[14], rk);
    return vs[14];
}
    