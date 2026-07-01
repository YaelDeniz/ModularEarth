import pandas as pd
import numpy as np
from mpl_toolkits.basemap import Basemap
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

# --- read the csv ---
df = pd.read_csv("tutorials/SubLayer_1_test.csv")

# --- set up the map ---
fig, ax = plt.subplots(figsize=(12, 6))
m = Basemap(projection='moll', lon_0=0, resolution='c', ax=ax)
m.drawcoastlines(linewidth=0.5)
m.drawparallels(range(-90, 91, 30), labels=[1,0,0,0])
m.drawmeridians(range(-180, 181, 60), labels=[0,0,0,1])

# --- plot each block ---
for block_id, group in df.groupby("block_id"):

    # get the angular limits for this block
    flag   = group["mod_flag"].iloc[0]
    phi1   = group["phi1"].iloc[0]
    phi2   = group["phi2"].iloc[0]
    theta1 = group["theta1"].iloc[0]
    theta2 = group["theta2"].iloc[0]

    # convert to geographic coordinates
    lon1 = phi1 - 180;   lon2 = phi2 - 180
    lat1 = 90 - theta1;  lat2 = 90 - theta2

    # build the four corners in the style you described
    # order: bottom-left, bottom-right, top-right, top-left
    lons = [lon1, lon2, lon2, lon1]
    lats = [lat2, lat2, lat1, lat1]   # lat2 < lat1 since theta2 > theta1

    # project to map coordinates
    x, y = m(lons, lats)

    # Plot points
    m.plot(x, y, 'o', markersize=3, color='steelblue')

    if flag == 1:

        # Top horizontal edge
        lons_top = np.linspace(lon1, lon2, 100)
        lats_top = np.ones_like(lons_top) * lat1

        # Bottom horizontal edge
        lons_bottom = np.linspace(lon1, lon2, 100)
        lats_bottom = np.ones_like(lons_bottom) * lat2

        # Left vertical edge
        lats_left = np.linspace(lat1, lat2, 100)
        lons_left = np.ones_like(lats_left) * lon1

        # Right vertical edge
        lats_right = np.linspace(lat1, lat2, 100)
        lons_right = np.ones_like(lats_right) * lon2

        # Project coordinates
        x_top, y_top = m(lons_top, lats_top)
        x_bottom, y_bottom = m(lons_bottom, lats_bottom)
        x_left, y_left = m(lons_left, lats_left)
        x_right, y_right = m(lons_right, lats_right)

        # Plot edges
        m.plot(x_top, y_top, color='blue')
        m.plot(x_bottom, y_bottom, color='red')
        m.plot(x_left, y_left, color='yellow')
        m.plot(x_right, y_right, color='forestgreen')

        # --- Fill region ---
        # Build polygon by going around the boundary: top -> right -> bottom (reversed) -> left (reversed)
        x_poly = np.concatenate([x_top, x_right, x_bottom[::-1], x_left[::-1]])
        y_poly = np.concatenate([y_top, y_right, y_bottom[::-1], y_left[::-1]])

        from matplotlib.patches import Polygon
        from matplotlib.collections import PatchCollection

        poly = Polygon(np.column_stack([x_poly, y_poly]), closed=True)
        patch = PatchCollection([poly], alpha=0.3, facecolor='blue', edgecolor='none')
        plt.gca().add_collection(patch)

    # optionally label the block at its centre
    lon_c = (lon1 + lon2) / 2
    lat_c = (lat1 + lat2) / 2
    xc, yc = m(lon_c, lat_c)
    ax.text(xc, yc, str(block_id+1), fontsize=6,
            ha='center', va='center', color='black')

plt.title("Blocks on Mollweide projection")
plt.tight_layout()
plt.savefig("blocks_map.png", dpi=150)
plt.show()