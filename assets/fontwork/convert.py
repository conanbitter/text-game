import json
import struct

raw_data = None
kerning_data = None

with open("font.json", 'r') as inf:
    raw_data = json.load(inf)

with open("kerning.json", 'r') as inf:
    kerning_data = json.load(inf)

outf = open("../font.fnt", 'wb')

em_size = raw_data['metrics']['emSize'] * raw_data['atlas']['size']

image_width = raw_data['atlas']['width']
image_height = raw_data['atlas']['height']
space_advance = 0.0
glyph_count = 0
glyphs = set()

for glyph_data in raw_data['glyphs']:
    if glyph_data['unicode'] == 32:
        space_advance = glyph_data['advance'] * em_size
    if 'atlasBounds' in glyph_data:
        glyph_count += 1
    glyphs.add(glyph_data['unicode'])

kerning = []
kern_emsize = kerning_data['emsize'] / em_size
for kd in kerning_data['pairs']:
    gleft = kd['left']
    gright = kd['right']
    kern = kd['kerning']
    if (gleft in glyphs) and (gright in glyphs):
        kerning.append({'left': gleft, 'right': gright, 'kerning': kern / kern_emsize})

sdf_range = raw_data['atlas']['distanceRange']
glyph_count = glyph_count

outf.write(struct.pack('ffLL', sdf_range, space_advance, glyph_count, len(kerning)))

for glyph_data in raw_data['glyphs']:
    glyph_code = glyph_data['unicode']
    if 'atlasBounds' not in glyph_data:
        continue
    glyph_rect_x = glyph_data['atlasBounds']['left']
    glyph_rect_y = image_height - glyph_data['atlasBounds']['top']
    glyph_rect_w = glyph_data['atlasBounds']['right'] - glyph_data['atlasBounds']['left']
    glyph_rect_h = glyph_data['atlasBounds']['top'] - glyph_data['atlasBounds']['bottom']

    # glyph_width = glyph_data['planeBounds']['right'] - glyph_data['planeBounds']['left']
    # glyph_height = glyph_data['planeBounds']['top'] - glyph_data['planeBounds']['bottom']
    glyph_originx = -glyph_data['planeBounds']['left'] * em_size  # / glyph_width * glyph_rect_w
    glyph_originy = glyph_data['planeBounds']['top'] * em_size  # / glyph_height * glyph_rect_h

    glyph_advance = glyph_data['advance'] * em_size

    # print(f"Code {glyph_code}")
    # print(f"Texture region (x:{glyph_rect_x}, y:{glyph_rect_y}, w:{glyph_rect_w}, h:{glyph_rect_h})")
    # print(f"Origin x:{glyph_originx} y:{glyph_originy}")
    # print(f"Advance {glyph_advance}")
    # print(glyph_data)
    outf.write(struct.pack('Lfffffff', glyph_code, glyph_rect_x, glyph_rect_y, glyph_rect_w, glyph_rect_h, glyph_originx, glyph_originy, glyph_advance))

for k in kerning:
    outf.write(struct.pack('LLf', k['left'], k['right'], k['kerning']))
outf.close()
