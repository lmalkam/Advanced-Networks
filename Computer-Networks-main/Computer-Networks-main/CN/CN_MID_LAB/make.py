import os


files = [
    # 'Editor',
    'r1',
    'r2',
    'r3',
    # 'Advertiser',
    # 'N1',
    # 'N2',
    # 'Screen',
    # 'Live'
]

def generate_init_files():
    for i in files:
        os.system('touch '+i+'.cpp')
    os.system('touch D.txt')
    os.system('touch cn_header.h')

def compile_files():
    for i in files:
        os.system('g++ '+i+'.cpp -o '+i)

if __name__ == '__main__':
    compile_files()