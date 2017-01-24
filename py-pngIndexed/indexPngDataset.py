from PIL import Image
import numpy as np
import sys

dataset = {
    'air_freshener': (1, [50, 0, 0]),
    'banana': (2, [0, 50, 0]),
    'big_dice': (3, [0, 0, 50]),
    'blue_vase': (4, [20, 0, 0]),
    'chocolate_syrup': (5, [0, 20, 0]),
    'cocacola': (6, [0, 0, 20]),
    'cofee_cup_1': (7, [30, 0, 0]),
    'cofee_cup_2': (8, [0, 30, 0]),
    'colacao': (9, [0, 0, 30]),
    'danone_yogurt': (10, [40, 0, 0]),
    'hand_crean': (11, [0, 40, 0]),
    'honey_pot':(12, [0, 0, 40]),
    'medicine_1':(13, [50, 0, 0]),
    'medicine_2':(14, [0, 50, 0]),
    'medicine_3':(15, [0, 0, 50]),
    'medicine_4':(16, [60, 0, 0]),
    'medicine_5':(17, [0, 60, 0]),
    'medicine_6':(18, [0, 0, 60]),
    'milk':(19, [70, 0, 0]),
    'plant':(20, [0, 70, 0]),
    'plastic_knife':(21, [0, 0, 70]),
    'plastic_plate':(22, [80, 0, 0]),
    'saccharine':(23, [0, 80, 0]),
    'taz':(24, [0, 0, 80]),
    'tea':(25, [90, 0, 0]),
    'telephone':(26, [0, 90, 0]),
    'toilet':(27, [0, 0, 90]),
    'wireless_telephone':(28, [100, 0, 0])}

def main(argv):
    arguments = len(argv)
    if arguments == 1:
        print "You must introduce the list of routes of the masks: python-script mask1 mask2 mask3..."
        sys.exit(0)    

    #Quantity of masks
    quantyMasks = arguments - 1

    #Create double palette colour
    max_colour = np.iinfo('uint8').max
    map_palette = [0]*(max_colour+1)*3
    indexed = np.array(Image.open(str(argv[1])).convert('P'))*0

    #Indexing mas in png
    for i in range(1, arguments):
        print "Processing", argv[i]
        splitted = argv[i].split('/')
        data = dataset[splitted[len(splitted)-1].split('.')[0]]
        im = Image.open(str(argv[i])).convert('P')
        mask = np.array(im)
        indexed[mask > max_colour/2] = data[0]
        map_palette[data[0]*3 : (data[0]+1)*3] = data[1]

    imfile = Image.fromarray(indexed)
    imfile.putpalette(map_palette)
    imfile.save('pngIndexed.png')


if __name__ == "__main__":
    main(sys.argv)
