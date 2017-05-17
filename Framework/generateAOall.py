import struct
import os
import os.path
import sys
import numpy
import scipy
import random
import math
from PIL import Image
import matplotlib.pyplot as plt

def readHeight(fname): # 90 degree
    #print 'rotating', fname
    f = open(fname, 'rb')

    header = f.read(256) # read headers
    version, w, h, spacing, ntex, texnamesize = struct.unpack('LLLfLL', header[:4*6])
    version, w, h, spacing, ntex, texnamesize,dw,dh,rt,rt2,rt3,rt4 = struct.unpack('LLLfLLLL4B', header[:36])
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

def normalize(vector3):
	sum = math.sqrt(vector3[0] * vector3[0] + vector3[1] * vector3[1] + vector3[2] * vector3[2])
	if(sum == 0):
		return [0.0,0.0,0.0]
	vector3[0] = vector3[0] / sum
	vector3[1] = vector3[1] / sum
	vector3[2] = vector3[2] / sum
	return vector3


def GenerateAO(offsetx, offsety, heights):
	height = heights[(offsetx + 1) * 53 + offsety + 1]
	height01 = heights[(offsetx + 1) * 53 + offsety]
	height21 = heights[(offsetx + 1) * 53 + offsety + 2]
	height10 = heights[offsetx * 53 + offsety + 1]
	height12 = heights[(offsetx + 2) * 53 + offsety + 1]
	x = [2.0,0.0,height21-height01]
	y = [0.0,2.0,height12-height10]
	normal = numpy.cross(x,y)
	normal = normalize(normal)

	if math.fabs(normal[0]) > math.fabs(normal[2]):
		binormal = [normal[2], 0.0, -normal[0]]
	else:
		binormal = [0.0, -normal[2], normal[1]]
	binormal = normalize(binormal)
	tangent = numpy.cross(binormal, normal)

	visibility = 0
	maxdistance = 6.0

	for i in range(4):
		for j in range(4):
			u0 = (i + random.random()) / 4
			v0 = (j + random.random()) / 4
			r = math.sqrt(u0)
			phi = 2.0 * math.pi * v0
			p = numpy.array([0.0,0.0,0.0])
			p[0] = r * math.cos(phi)
			p[1] = r * math.sin(phi)
			p[2] = math.sqrt(max(0.0, 1.0 - p[0] * p[0] - p[1]* p[1]))
			q =  numpy.array([0.0,0.0,0.0])
			q[0] = p[0] * tangent[0] + p[1] * binormal[0] + p[2] * normal[0]
			q[1] = p[0] * tangent[1] + p[1] * binormal[1] + p[2] * normal[1]
			q[2] = p[0] * tangent[2] + p[1] * binormal[2] + p[2] * normal[2]
			p = normalize(q)
			origin = numpy.array([1.0 * offsety, 1.0* offsetx,1.0 *height])
			ray_visibility = 1.0;
			for samples in range(8):
				new_pos = numpy.array([0.0,0.0,0.0])
				new_pos[0] = origin[0] + (samples+1)*(maxdistance / 8.0) * p[0]
				new_pos[1] = origin[1] + (samples+1)*(maxdistance / 8.0) * p[1]
				new_pos[2] = origin[2] + (samples+1)*(maxdistance / 8.0) * p[2]
				if(new_pos[0] < -1 or new_pos[0] >= 52 or new_pos[1] < -1 or new_pos[1] >= 52):
					continue
				new_height = heights[(int(new_pos[1] + 1)) * 53 + (int(new_pos[0] + 1))]
				if new_height >= new_pos[2]:
					ray_visibility *= 0
				else:
					ray_visibility *= 1
			visibility += ray_visibility
	return 1.0 - float(visibility / 16.0)


rootdir = "data\\"
for parent,dirnames,filenames in os.walk(rootdir):
	for dirname in dirnames:
		filename = filename = "data\\" + dirname + "\\terrain"
		print dirname,filename
		AOfile = "data\\" + dirname + "\\AO"
		AOfiletxt = "data\\" + dirname + "\\AO.txt"
		w,h,heights = readHeight(filename)
		newheights = numpy.zeros(w*h)
		for i in range(h):
			for j in range(w):
				height = heights[i*w+j]
				if height < 0:
					height = -height
				newheights[i*w+j] = height
		file_object = open(AOfiletxt, 'w')
		file_binary = open(AOfile, 'wb')
		aovalue = numpy.zeros((51,51))
		for i in range(51):
			for j in range(51):
				aovalue[i,j] = GenerateAO(i,j,newheights)
				file_object.write("%f " % aovalue[i,j])
				file_binary.write(struct.pack('f',aovalue[i,j]))
			file_object.write("\n")
		file_object.close()
		file_binary.close()
