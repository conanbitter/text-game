import json
import struct

font_data = None
kerning_data = None

with open("font.json", 'r') as inf:
    font_data = json.load(inf)

with open("kerning.json", 'r') as inf:
    kerning_data = json.load(inf)

outf = open("../font.fnt", 'wb')

em_size = font_data['metrics']['emSize'] * font_data['atlas']['size']

image_width = font_data['atlas']['width']
image_height = font_data['atlas']['height']
space_advance = 0.0
glyph_count = 0
glyphs = set()

for glyph_data in font_data['glyphs']:
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

sdf_range = font_data['atlas']['distanceRange']
glyph_count = glyph_count
font_size = font_data['atlas']['size']
line_height = font_data['metrics']['lineHeight'] * font_size

# Writing file

outf.write(b"FONT")  # Magic
outf.write(struct.pack('B', 1))  # Version

# Header

outf.write(b"HEAD")
outf.write(struct.pack('I', 4 * 4))
outf.write(struct.pack('ffff', font_size, line_height, sdf_range, space_advance))

# Font texture

TEXTURE_FILENAME = b"font.png"

outf.write(b"IMAG")
outf.write(struct.pack('I', len(TEXTURE_FILENAME)))
outf.write(TEXTURE_FILENAME)

# Glyphs

outf.write(b"CHAR")
outf.write(struct.pack('I', glyph_count * 8 * 4))

for glyph_data in font_data['glyphs']:
    glyph_code = glyph_data['unicode']
    if 'atlasBounds' not in glyph_data:
        continue
    glyph_rect_x = glyph_data['atlasBounds']['left']
    glyph_rect_y = image_height - glyph_data['atlasBounds']['top']
    glyph_rect_w = glyph_data['atlasBounds']['right'] - glyph_data['atlasBounds']['left']
    glyph_rect_h = glyph_data['atlasBounds']['top'] - glyph_data['atlasBounds']['bottom']

    glyph_originx = -glyph_data['planeBounds']['left'] * em_size
    glyph_originy = glyph_data['planeBounds']['top'] * em_size

    glyph_advance = glyph_data['advance'] * em_size

    # print(f"Code {glyph_code}")
    # print(f"Texture region (x:{glyph_rect_x}, y:{glyph_rect_y}, w:{glyph_rect_w}, h:{glyph_rect_h})")
    # print(f"Origin x:{glyph_originx} y:{glyph_originy}")
    # print(f"Advance {glyph_advance}")
    # print(glyph_data)
    outf.write(struct.pack('Ifffffff', glyph_code, glyph_rect_x, glyph_rect_y, glyph_rect_w, glyph_rect_h, glyph_originx, glyph_originy, glyph_advance))

# Kerning

outf.write(b"KERN")
outf.write(struct.pack('I', len(kerning) * 3 * 4))

for k in kerning:
    outf.write(struct.pack('LLf', k['left'], k['right'], k['kerning']))

outf.close()
