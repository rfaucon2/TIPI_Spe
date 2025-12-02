from astropy.io import fits
import matplotlib.pyplot as plt
import numpy as np

hdu = fits.open("stars_data.fits")
data = hdu[1].data

radii = list(data.field("radius_bstep"))
mass = list(data.field("m_act_bstep"))

def show_x_y():
    plt.plot(radii, mass, "x")
    plt.ylabel("mass (m_S)")
    plt.xlabel("radii (r_S)")
    plt.xscale('log')
    plt.yscale('log')
    plt.show()

def show_histo():
    fig, axs = plt.subplots(1, 2, sharey=False, tight_layout=True)
    n_bins = 10**3
    
    # Get log width for histogram
    _, bins, _ = plt.hist(radii, bins = n_bins)
    axs[1].clear()
    logbins = np.logspace(np.log10(bins[0]),np.log10(bins[-1]),len(bins))
    
    # Set radius histo
    axs[0].hist(radii, bins=logbins)
    axs[0].set_yscale("log")
    axs[0].set_xscale("log")

    # Set mass histo
    axs[1].hist(mass, bins=logbins)
    axs[1].set_yscale("log")
    axs[1].set_xscale("log")
    
    plt.show()

show_histo()
hdu.close()
