from rootalias import *
import math

density = 7.87
f_step = TFile('data/dedx.root')
h_dedx = TH2D('h_dedx', 'h_dedx', 90, 0, 9, 100, 0, 100)
for step in f_step.TestEm17:
    h_dedx.Fill(math.log10(step.energy), step.dEdx / density)
p_dedx = h_dedx.ProfileX()

c1 = TCanvas('c1', 'c1', 800, 600)
gPad.SetLogx()
set_margin()
set_h2_style(h_dedx)
h_dedx.Draw('colz')

p_dedx.SetLineWidth(2)
p_dedx.SetLineColor(kRed)
p_dedx.Draw('sames')

c1.Update()
c1.SaveAs('figures/dedx.pdf')
raw_input('Press any key to continue.')
