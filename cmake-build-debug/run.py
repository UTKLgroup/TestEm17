from subprocess import call
import numpy as np
import math

for energy in np.arange(1, 8.5, 0.5):
    energy = 10**energy

    with open('test_run.mac', 'w') as f_run:
        f_run.write('/control/verbose 2\n')
        f_run.write('/run/verbose 2\n')
        f_run.write('/testem/det/setMat Iron\n')
        f_run.write('/testem/det/setSize 1 m\n')
        f_run.write('/testem/phys/addPhysics emstandard_opt0\n')
        f_run.write('/run/setCut 1 mm\n')
        f_run.write('/run/initialize\n')
        f_run.write('/gun/particle mu-\n')
        f_run.write('/gun/energy {} MeV\n'.format(energy))
        f_run.write('/testem/histo/setFileName allproc\n')
        f_run.write('/testem/histo/setHisto 1 100 0. 10000.\n')
        f_run.write('/run/printProgress 10000\n')
        f_run.write('/run/beamOn 10000\n')

    call('./TestEm17 test_run.mac', shell=True)
    call('mv allproc.root data/run{}.root'.format(math.log10(energy)), shell=True)
