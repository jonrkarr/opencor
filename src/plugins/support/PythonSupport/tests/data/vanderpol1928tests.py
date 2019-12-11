import opencor as oc
import sys

sys.dont_write_bytecode = True

from utils import *

if __name__ == '__main__':
    # Test the van der Pol 1928 model using different solvers

    run_simulations('van_der_pol_model_1928.cellml', 'van der Pol 1928 model')
