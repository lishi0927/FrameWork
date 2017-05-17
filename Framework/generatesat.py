import struct
import os
import sys

def readHeight(fname): # 90 degree
    #print 'rotating', fname
    f = open(fname, 'rb')

    header = f.read(256) # read headers
    version, w, h, spacing, ntex, texnamesize = struct.unpack('IIIfII', header[:4*6])
    version, w, h, spacing, ntex, texnamesize,dw,dh,rt,rt2,rt3,rt4 = struct.unpack('IIIfIIII4B', header[:36])

    texnames = []
    for i in range(ntex): # extract tex names
        texname = f.read(texnamesize)
        texname = texname.replace('\x00','')
        texnames.append(texname)
    #print w,h
    assert(w==h)
    assert(w>0)
    assert(ntex>0)
    assert(texnamesize==128)
    heights = struct.unpack('f'*w*h, f.read(4*w*h))
    #print w,h,heights
    return w,h,heights

def tohex(val, nbits = 16):
    return (val + (1 << nbits)) % (1 << nbits)

def getEle(ret, i, j, w):
    if (i < 0 or j < 0):
        return 0
    return ret[i * w + j]

xmin = 15
ymin = -24
size = 50

inf = 1000000

hoff = 280
htop = 1300

ep = 60

def transform(x):
    return pow((x + hoff) / htop, ep)

def transform1(x):
    return pow(1 - ((x + hoff) / htop), ep)

def restore(x, tot):
    return pow(x, 1.0 / ep) * htop - hoff

def restore1(x, tot):
    return (1 - pow(x / tot, 1.0 / ep)) * htop - hoff

def writeMat(dirname, maxOrMin, data, w, h):
    if (maxOrMin == 0):
        tFile = "data/" + dirname + "/maxSAT"
    else:
        tFile = "data/" + dirname + "/minSAT"

    output = open(tFile, "wb")
    output.write(struct.pack("L" * 5, w, h, htop, hoff, ep))
    output.write(struct.pack("d" * len(data), *data))

for i in range(xmin, xmin + size):
    for j in range(ymin, ymin + size):
        lstr = '%04x' % tohex(i)
        rstr = '%04x' % tohex(j)
        dirname = lstr + rstr

        filename = "data/" + dirname + "/terrain"

        w, h, heights = readHeight(filename)

        ma = -inf
        mi = inf

        ret = []
        ret1 = []

        for k1 in range(1, h - 1):
            for k2 in range(1, w - 1):
                height = transform(heights[k1 * w + k2])
                height1 = transform1(heights[k1 * w + k2])
                ret.append(getEle(ret, k1 - 2, k2 - 1, w - 2) + \
                           getEle(ret, k1 - 1, k2 - 2, w - 2) - \
                           getEle(ret, k1 - 2, k2 - 2, w - 2) + \
                           height)
                ret1.append(getEle(ret1, k1 - 2, k2 - 1, w - 2) + \
                            getEle(ret1, k1 - 1, k2 - 2, w - 2) - \
                            getEle(ret1, k1 - 2, k2 - 2, w - 2) + \
                            height1)

        writeMat(dirname, 0, ret, w - 2, h - 2)
        writeMat(dirname, 1, ret1, w - 2, h - 2)