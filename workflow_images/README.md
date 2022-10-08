# Programming Flow (inputs/outputs)

## midrapidity open heavy flavor TSSA (p+p 200 GeV)

- `ohfe_pp_asymmetry.gv.png` depicts the programming flow for calculating the TSSA before background correction from the taxi output ntuple
- `ohfe_pp_crossChecks.gv.png` depicts the programming flow for the bin average calculation, sin(phi) modulation cross check, and bunch shuffling cross check from the `curated_ohfe.root` ntuple (see `ohfe_pp_asymmetry.gv.png`)  
- `ohfe_pp_bgFracs.gv.png` depicts the programming flow for calculating the background fractions in each pT bin from the taxi output ntuple and PISA simulations conducted in PPG223 (some simulations were redone to calculate the bg fracs for the separate charges)
- `ohfe_pp_bgCorrection.gv.png` depicts the programming flow for the background correction procedure from the AN(pT) graphs `ohfe_AN_**.root` (see `ohfe_pp_asymmetry.gv.png`) and the background fraction calculation (see `ohfe_pp_bgFracs.gv.png`)
- `ohfe_pp_sims.gv.png` depicts the programming flow for the theory comparisons and parameter constraint studies, using the background corrected AN(pT) graphs `bgCorrected_ohfe_AN_**.root` as an input (see `ohfe_pp_bgCorrection.gv.png`)

### midrapidity neutral pion and eta meson TSSA (p+Au 200 GeV, p+Al 200 GeV)

- `pi0_eta_pA.gv.png` depicts the programming flow for calculating the TSSA before background correction from the taxi output ntuple (NEED TO UPDATE NAME!)

