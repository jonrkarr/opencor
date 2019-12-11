import opencor as oc
import sys

sys.dont_write_bytecode = True

from utils import *

if __name__ == '__main__':
    # Test for a local/remote CellML file

    test_simulation('Local CellML file', 'tests/cellml/lorenz.cellml')
    test_simulation('Remote CellML file',
                    'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/cellml/lorenz.cellml', False)

    # Test for a local/remote SED-ML file

    test_simulation('Local SED-ML file', 'tests/sedml/lorenz.sedml', False)
    test_simulation('Remote SED-ML file',
                    'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/sedml/lorenz.sedml', False)

    # Test for a local/remote COMBINE archive

    test_simulation('Local COMBINE archive', 'tests/combine/lorenz.omex', False)
    test_simulation('Remote COMBINE archive',
                    'https://raw.githubusercontent.com/opencor/opencor/master/models/tests/combine/lorenz.omex', False)
