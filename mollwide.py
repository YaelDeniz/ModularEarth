from mpl_toolkits.basemap import Basemap
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon




# lon_0 is central longitude of projection.
# resolution = 'c' means use crude resolution coastlines.
m = Basemap(projection='moll',lon_0=0,resolution='c')
m.drawcoastlines()
m.fillcontinents(color='coral',lake_color='aqua')
# draw parallels and meridians.
m.drawparallels(np.arange(-90.,120.,10.))
m.drawmeridians(np.arange(0.,420.,10.))
m.drawmapboundary(fill_color='aqua') 


"""
I need to add a list of polygon edges


"""

# Define bounding box (e.g., a "bin" between 35N-45N and 100W-80W)
lons = [-180, -80, -80, -180]
lats = [35, 35, 55, 55]
x, y = m(lons, lats)
xy = list(zip(x, y))

# Create and add polygon
poly = Polygon(xy, facecolor='red', alpha=0.5)
plt.gca().add_patch(poly)

print("Polygon vertices (x, y):", xy)
plt.title("Mollweide Projection")
plt.show()