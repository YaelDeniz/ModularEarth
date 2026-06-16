# ModularEarth
ModularEarth is a TGeoManager (CERN ROOT) based application that creates 3D models based on PREM .txt files. This application allows you to customize the density and composition in localized regions of different layers of the Earth, representing heterogeneties in those layers.

#NOTES

In Block class, IsValid() Flag invalid when Z,A, or Density is < 0 meaning that 0.0 (Vacuum) is not accepted....

