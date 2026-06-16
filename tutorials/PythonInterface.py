import ROOT
import numpy as np
import matplotlib.pyplot as plt

ROOT.gSystem.Load("/home/ydenizhernandez/ModularEarth/libModularEarth.so")

ME = ROOT.ModularEarth  # namespace shortcut

def check_volume_media(volume):
    if not volume:
        print("Null volume!")
        return
    if not volume.GetMedium():
        print(f"Volume {volume.GetName()} has NO medium!")
    for i in range(volume.GetNdaughters()):
        node = volume.GetNode(i)
        if node:
            check_volume_media(node.GetVolume())



# --- Helper: build a std::vector<ModularEarth::SublayerConfig> in Python
def make_sublayer_configs(rows):
    v = ROOT.std.vector[ME.SublayerConfig]()

    for (idx, rmin, rmax, rho, Z, A, name) in rows:
        cfg = ME.SublayerConfig(
            int(idx),
            float(rmin),
            float(rmax),
            float(rho),
            float(A),
            float(Z),
            str(name)
        )
        v.push_back(cfg)

    return v


# --- Geometry manager like your macro (optional for plotting, but matches tutorial)
geo = ROOT.TGeoManager("TopVol", "Earth in Sphere demo")
matVac = ROOT.TGeoMaterial("Vacuum", 0, 0, 0)
medVac = ROOT.TGeoMedium("Vacuum", 1, matVac)
top = ROOT.TGeoVolume("EarthTopSphere", ROOT.TGeoSphere("Sphere", 0, 7000), medVac)
geo.SetTopVolume(top)

# --- OuterCore example (matches your Code 2)
OuterCore = ME.Layer("OuterCore", 1, 10, 10)
oc_rows = [
    (1, 1221.5, 1946.7, 11.9512, 0.4691, 1, "FeNi"),
    (2, 1946.7, 2475.1, 11.4726, 0.4691, 1, "FeNi"),
    #(3, 2475.1, 2900.3, 10.9884, 0.4691, 1, "FeNi"),
    #(4, 2900.3, 3260.6, 10.5008, 0.4691, 1, "FeNi"),
    #(5, 3260.6, 3480.0, 10.0861, 0.4691, 1, "FeNi"),
]
OuterCore.SetSubLayers(make_sublayer_configs(oc_rows))
OuterCore.BuildLayer()
"""
# --- UpperMantle + update blocks like your Code 2
UpperMantle = ME.Layer("UpperMantle", 3, 30, 60)
um_rows = [
    (1, 5701.0, 5971.0, 3.8983, 0.4954, 1, "SilicateUM"),
    (2, 5971.0, 6346.6, 3.4306, 0.4691, 1, "SilicateUM"),
]
UpperMantle.SetSubLayers(make_sublayer_configs(um_rows))

sublayerIndex = 1
blockIDs = ROOT.std.vector["int"]()
for b in [3, 7, 12]:
    blockIDs.push_back(b)

newA = ROOT.std.vector["double"]()
newZ = ROOT.std.vector["double"]()
newDensity = ROOT.std.vector["double"]()
for a in [28.0, 28.0, 28.0]:
    newA.push_back(a)
for z in [14.0, 14.0, 14.0]:
    newZ.push_back(z)
for d in [3.5, 3.6, 3.4]:
    newDensity.push_back(d)

UpperMantle.UpdateBlocksInSubLayer(
    sublayerIndex,
    blockIDs,
    "AnomalousMantle",
    newA,
    newZ,
    newDensity
)

UpperMantle.SubLayerSummary(sublayerIndex)
UpperMantle.BuildLayer()
"""
# --- Add volumes to top (like your macro)
top.AddNode(OuterCore.GetLayerVolume(), 1, ROOT.TGeoTranslation(0, 0, 0))
#top.AddNode(UpperMantle.GetLayerVolume(), 1, ROOT.TGeoTranslation(0, 0, 0))

geo.SetVerboseLevel(4)  # Full debug
geo.CheckOverlaps(0.001)  # Check for overlapping volumes (optional)
geo.CheckGeometry()       # Explicit call before CloseGeometry()
#check_volume_media(geo.GetTopVolume())
geo.CloseGeometry()
# geo.SetTopVisible()


"""
# --- Matplotlib plot: example = plot configured densities (OuterCore sublayers)
oc_density = np.array([row[3] for row in oc_rows], dtype=float)
oc_rmid = np.array([(row[1] + row[2]) / 2.0 for row in oc_rows], dtype=float)

plt.figure()
plt.plot(oc_rmid, oc_density, marker="o")
plt.xlabel("radius midpoint (km)")
plt.ylabel("density (g/cm^3)")
plt.title("OuterCore density profile (from config)")
plt.show()
"""

# --- Optional: ROOT draw (needs display; avoid on batch nodes)
# c = ROOT.TCanvas("c", "Geometry", 1200, 900)
# top.Draw("ogl")  # safer than "gl" in many environments
# c.Update()
# input("Press Enter to close...")
