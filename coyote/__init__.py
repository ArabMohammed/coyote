from dataclasses import dataclass
from inspect import signature
from sys import stderr
from warnings import warn
from .coyote_ast import CompilerV2, Var
from .vectorize_circuit import vectorize

@dataclass
class matrix:
    rows: int
    cols: int
    replicate: bool = False

@dataclass
class vector:
    size: int
    replicate: bool = False
    pack: bool = False

@dataclass
class scalar:
    pass

class copy_vector:
    def __init__(self, size, name, var_conv='{name}:{i}'):
        self.size = size
        self.name = name
        self.var_conv = var_conv

    def __getitem__(self, i):
        if i >= self.size:
            raise IndexError('list index out of range')
        return Var(self.var_conv.format(name=self.name, i=i))

    def __len__(self):
        return self.size
class copy_matrix:
    def __init__(self, rows, cols, name, var_conv='{name}:{i},{j}'):
        self.name = name
        self.rows = rows
        self.cols = cols 
        self.var_conv = var_conv

    def __getitem__(self, i):
        if i >= self.rows:
            raise IndexError('list index out of range')
        return copy_vector(self.cols, f'{self.name}:{i}', var_conv=f'{self.name}:{i};{{i}}')

    def __len__(self):
        return self.rows

    
class coyote_compiler:
    def __init__(self):
        self.func_signatures = {} 
        self.outputs = []

    def vectorize(self, lanes_out=[]):
        return vectorize(self.compiler, lanes_out)

    # * funcs is a list of function names that we want to compile
    def instantiate(self, *funcs, _input_groups=None, _force_lanes=None):
        self.outputs = []
        print(f"==> funcs in instantiate : {funcs}")
        input_groups, force_lanes, outputs = self.get_outputs(funcs)
    
        self.compiler = CompilerV2(input_groups, force_lanes=force_lanes)

        counter = 0
        for out in outputs:
            counter +=1
            print(counter)
            print(out)
            self.outputs.append(self.compiler.compile(out).val)
        #print(f"==> {[' '.join(f'%{reg}' for reg in self.outputs)] + list(map(str, self.compiler.code))}")
        """
        ['%5 %11 %17 %23', '%0 = hamming_distance_4(a):0 ~ hamming_distance_4(a):0', 
        '%1 = hamming_distance_4(b):0 ~ hamming_distance_4(b):0', '%2 = %0 + %1', '%3 = %0 * %1', '%4 = 2 * %3', 
        '%5 = %2 - %4', 
        '%6 = hamming_distance_4(a):1 ~ hamming_distance_4(a):1', '%7 = hamming_distance_4(b):1 ~ hamming_distance_4(b):1',
        '%8 = %6 + %7', '%9 = %6 * %7', '%10 = 2 * %9', '%11 = %8 - %10', '%12 = hamming_distance_4(a):2 ~ hamming_distance_4(a):2', 
        '%13 = hamming_distance_4(b):2 ~ hamming_distance_4(b):2', '%14 = %12 + %13', '%15 = %12 * %13', '%16 = 2 * %15',
        '%17 = %14 - %16', '%18 = hamming_distance_4(a):3 ~ hamming_distance_4(a):3', 
        '%19 = hamming_distance_4(b):3 ~ hamming_distance_4(b):3', '%20 = %18 + %19', '%21 = %18 * %19', 
        '%22 = 2 * %21', '%23 = %20 - %22']
        """
        return [' '.join(f'%{reg}' for reg in self.outputs)] + list(map(str, self.compiler.code))


    def get_outputs(self, funcs):
        input_groups = []
        force_lanes = {}
        outputs = []
        """
        if funcs is an empty list, it uses self.func_signatures, 
        which contains all the function signatures 
        that were previously decorated with @coyote.define_circuit
        """
        if len(funcs) == 0:
            funcs = self.func_signatures
        else:
            funcs = list(filter(lambda func: func.__name__ in funcs, self.func_signatures.keys()))

        for func in funcs:
            # function inputs 
            signature = self.func_signatures[func]
            params = {}
            for _p, t in signature.items():
                p = f'{func.__name__}({_p})'
                # p : test(c1) , _p : c1 , t.pack : False, t.replicate : True
                print(f"p : {p} , _p : {_p} , t.pack : {t.pack}, t.replicate : {t.replicate}")
                if isinstance(t, matrix):
                    if t.replicate:
                        print("=> replicate is true")
                        params[_p] = copy_matrix(t.rows, t.cols, p)
                    else:
                        params[_p] = [[Var(f'{p}:{i};{j}') for i in range(t.rows)] for j in range(t.cols)]
                    input_groups.append({f'{p}:{i};{j}' for i in range(t.rows) for j in range(t.cols)})
                elif isinstance(t, vector):
                    
                    if t.replicate and t.pack:
                        raise TypeError('replicate and pack options not allowed together!')
                    
                    if t.replicate:
                        print("=> replicate is true")
                        params[_p] = copy_vector(t.size, p)
                    else:
                        params[_p] = [Var(f'{p}:{i}') for i in range(t.size)]
                    ### input_groups.append([test(c1):0, test(c1):1, test(c1):2])
                    input_groups.append({f'{p}:{i}' for i in range(t.size)})
                    
                    if t.pack:
                        force_lanes.update({f'{p}:{i}': i for i in range(t.size)})
                else:
                    params[_p] = Var(p)
            #################################
            print(f"func type : {type(func)}")
            out = func(**params)
            print(f"function output : {out} \n")
            # [Op("+", Op("*", Var("test1(c1):0"), Var("test1(c2):0")), Op("+", Var("test1(c3):0"), Var("test1(c4):0")))] 
            
            #  [ 
            # 
            #  Op("-", Op("+", Var("hamming_distance_4(a):0"), Var("hamming_distance_4(b):0")), 
            #  Op("*", Var("2"), Op("*", Var("hamming_distance_4(a):0"), Var("hamming_distance_4(b):0")))), 
            # 
            # Op("-", Op("+", Var("hamming_distance_4(a):1"), Var("hamming_distance_4(b):1")), 
            # Op("*", Var("2"), Op("*", Var("hamming_distance_4(a):1"), Var("hamming_distance_4(b):1")))), 
            # 
            # Op("-", Op("+", Var("hamming_distance_4(a):2"), Var("hamming_distance_4(b):2")), 
            # Op("*", Var("2"), Op("*", Var("hamming_distance_4(a):2"), Var("hamming_distance_4(b):2")))),
            # 
            # Op("-", Op("+", Var("hamming_distance_4(a):3"), Var("hamming_distance_4(b):3")), 
            # Op("*", Var("2"), Op("*", Var("hamming_distance_4(a):3"), Var("hamming_distance_4(b):3"))))
            # 
            # ]
            
            print(f"==> input_groups : {input_groups} \n")
            [{'test1(c1):0','test1(c1):1'}, {'test1(c2):0','test1(c2):1'}, {'test1(c3):0'}, {'test1(c4):0'}]
            if isinstance(out, list) or isinstance(out, tuple):
                outputs += out
            else:
                outputs.append(out)
        return input_groups, force_lanes, outputs
#######################################################################################################
#######################################################################################################
    def define_circuit(self, **types):
        
        def wrapper(func):
        
            name = func.__name__
            allowed_types = [matrix, vector, scalar]

            params = signature(func).parameters

            for p in types:
                if p not in params:
                    raise TypeError(f'In function {name}: no such parameter {p}')
                if not any(isinstance(types[p], t) for t in allowed_types): 
                    raise TypeError(f'In function {name}: {types[p]} is not an allowed type')



            if any(param.kind == param.VAR_KEYWORD for param in params.values()):
                raise TypeError(f'In function {name}: keyword arguments not allowed')

            if any(param.kind == param.VAR_POSITIONAL for param in params.values()):
                raise TypeError(f'In function {name}: positional arguments not allowed')

            for p in params:
                if p not in types:
                    types[p] = scalar()
                    warn(f'In function {name}: No type provided for {p}, assuming scalar')

            self.func_signatures[func] = types
            return func

        return wrapper


def recursive_sum(vals):
    if len(vals) == 1:
        return vals[0]
    mid = len(vals) // 2
    return recursive_sum(vals[:mid]) + recursive_sum(vals[mid:])


def alternating_sum(vals):
    if len(vals) == 1:
        return vals[0]
    mid = len(vals) // 2
    if mid % 2:
        return alternating_sum(vals[:mid]) - alternating_sum(vals[mid:])
    return alternating_sum(vals[:mid]) + alternating_sum(vals[mid:])

if __name__ == '__main__':
    coyote = coyote_compiler()

    # VEC_SIZE = 8
    # @coyote.define_circuit(a=vector(VEC_SIZE), b=vector(VEC_SIZE))
    # def dot_product(a, b):
    #     return recursive_sum([a[i] * b[i] for i in range(VEC_SIZE)])

    MAT_SIZE = 2
    @coyote.define_circuit(a=matrix(MAT_SIZE, MAT_SIZE), b=matrix(MAT_SIZE, MAT_SIZE))
    def matrix_multiply(a, b):
        output = []
        for i in range(MAT_SIZE):
            row = []
            for j in range(MAT_SIZE):
                row.append(recursive_sum([a[i][k] * b[k][j] for k in range(MAT_SIZE)]))
            output += row

        return output
        # return output[0] * output[3] - output[1] * output[2]


    # sz = 3
    # @coyote.define_circuit(arr1=vector(sz), arr2=vector(sz))
    # def get_distances(arr1, arr2):
    #     dists = []
    #     for i in range(sz):
    #         for j in range(sz):
    #             xdiff = arr1[i] - arr2[j]
    #             # xdiff = plus(f'{arr1}_x{i}', f'{arr2}_x{j}')
    #             dists.append(xdiff * xdiff)
    #             # dists.append(times(xdiff, copy(xdiff)))
    #             # print(dists[-1])
    #     return dists

    # @coyote.define_circuit(a=scalar(), b=scalar())
    # def func(a, b):
    #     x = a * b
    #     y = x + a
    #     z = x * x
    #     return y + z

    # IMG_SIZE = 3
    # KER_SIZE = 2
    # @coyote.define_circuit(ker=matrix(KER_SIZE, KER_SIZE), img=matrix(IMG_SIZE, IMG_SIZE))
    # def convolve(ker, img):
    #     output = []
    #     for i in range(IMG_SIZE - KER_SIZE + 1):
    #         for j in range(IMG_SIZE - KER_SIZE + 1):
    #             vals = []
    #             for k1 in range(KER_SIZE):
    #                 for k2 in range(KER_SIZE):
    #                     vals.append(img[i + k1][j + k2] * ker[k1][k2])
    #             output.append(recursive_sum(vals))
    #     return output

    total_rotates = []
    for i in range(20):
        scalar_code = coyote.instantiate()
        vectorized_code = coyote.vectorize()
        print('\n'.join(scalar_code))
        print(ans := '\n'.join(map(str, vectorized_code)))
        total_rotates.append(ans.count('>>'))

    print(sum(total_rotates) / 20, min(total_rotates), max(total_rotates), total_rotates)
        
    