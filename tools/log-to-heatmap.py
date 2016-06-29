#! /usr/bin/env python
import json
import os
import sys
import colorsys
import re

cr_coord_map = [
    [
        # Row 0
        [ 4,  0], [ 4,  2], [ 2,  0], [ 1,  0], [ 2,  2], [ 3,  0], [ 3,  2],
        [ 3,  4], [ 3,  6], [ 2,  4], [ 1,  8], [ 2,  6], [ 4,  4], [ 4,  6],
    ],
    [
        # Row 1
        [ 8,  0], [ 8,  2], [ 6,  0], [ 5,  0], [ 6,  2], [ 7,  0], [ 7,  2],
        [ 7,  4], [ 7,  6], [ 6,  4], [ 5,  2], [ 6,  6], [ 8,  4], [ 8,  6],
    ],
    [
        # Row 2
        [12,  0], [12,  2], [10,  0], [ 9,  0], [10,  2], [11, 0], [     ],
        [      ], [11,  2], [10,  4], [ 9,  2], [10,  6], [12, 4], [12, 6],
    ],
    [
        # Row 3
        [17,  0], [17,  2], [15,  0], [14,  0], [15,  2], [16,  0], [13,  0],
        [13,  2], [16,  2], [15,  4], [14,  2], [15,  6], [17,  4], [17,  6],
    ],
    [
        # Row 4
        [20,  0], [20,  0], [19,  0], [18,  0], [19,  2],
        [19,  4], [18,  2], [19,  6], [20,  3], [20,  3],
    ],
    [
        # Row 5
        [     ], [23,  0], [22,  2], [22,  0], [22,  4], [21,  0], [21,  2],
        [24, 0], [24,  0], [25,  0], [25,  4], [25,  2], [26,  0], [      ],
    ],
]

def set_attr_at(j, b, n, attr, fn, val):
    blk = j[b][n]
    if attr in blk:
        blk[attr] = fn(blk[attr], val)
    else:
        blk[attr] = fn(None, val)

def coord(col, row):
    return cr_coord_map[row][col]

def set_attr(orig, new):
    return new

def set_bg(j, (b, n), color):
    set_attr_at(j, b, n, "c", set_attr, color)
    #set_attr_at(j, b, n, "g", set_attr, False)

def hls_to_hex(h, l, s):
    r, g, b = colorsys.hls_to_rgb(max(0, min(h, 360)), min(s, 1.0), max (0, min (l, 0.75)))
    r, g, b = [x * 255 for x in r, g, b]
    return "#%02x%02x%02x" % (r, g, b)

def heatmap_color (v):
    return hls_to_hex (((1.0 - v) * 240) / 360, 1, 0.5)

with open(sys.argv[1], "r") as f:
    layout = json.load (f)

## Reset colors
for row in cr_coord_map:
    for col in row:
        if col != []:
            set_bg (layout, col, "#d9dae0")
            #set_attr_at (layout, col[0], col[1], "g", set_attr, True)

# Load the keylog
def load_keylog(fname):
    keylog = {}
    total = 0
    with open(fname, "r") as f:
        lines = f.readlines()
    for line in lines:
        m = re.search ('KL: col=(\d+), row=(\d+)', line)
        if not m:
            continue
        (c, r) = (int(m.group (2)), int(m.group (1)))
        if (c, r) in keylog:
            keylog[(c, r)] = keylog[(c, r)] + 1
        else:
            keylog[(c, r)] = 1
        total = total + 1
    return total / 2, keylog

total, log = load_keylog (sys.argv[2])

def l_flat(s):
    f = s.split("\n")
    return ", ".join (f)

# Create the heatmap
for (c, r) in log:
    coords = coord(c, r)
    b, n = coords
    cap = (total + len(log)) / 2
    v = float(log[(c, r)]) / cap
    print >> sys.stderr, "%s => %d/%d => %f = %s" % (l_flat(layout[b][n+1]), log[(c,r)], cap, v, heatmap_color(v))
    set_bg (layout, coord(c, r), heatmap_color (v))

print json.dumps(layout)
