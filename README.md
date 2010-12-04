Python 2D perlin noise generator as a C extension.

Based off of examples by [Hugo Elias](http://freespace.virgin.net/hugo.elias/models/m_perlin.htm)

# Usage
    import noise, itertools
    for x,y in itertools.product(range(256), range(256)):
      noise.pnoise2d(x,y,[octaves])
