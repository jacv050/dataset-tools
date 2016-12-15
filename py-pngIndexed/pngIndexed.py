from PIL import Image
import numpy as np
import sys



def main(argv):
    arguments = len(argv)
    if arguments == 1:
        print "You must introduce the list of routes of the masks: python-script mask1 mask2 mask3..."
        sys.exit(0)    

    #Quantity of masks
    quantyMasks = arguments - 1
    #Read first image
    print "Processing", str(argv[1])
    im = Image.open(str(argv[1])).convert('P')
    mask = np.array(im)

    #Create double palette colour
    max_colour = np.iinfo(mask.dtype).max
    map_palette = [0]*(max_colour+1)*3
    #(np.iinfo(mask.dtype).max+1)*3

    #Define image indexed and map
    indexed = mask
    indexed[indexed > max_colour/2] = 1
    #Define colour for first mask
    auxMap = [50, 0, 0]
    map_palette[3:6] = auxMap

    #Indexing mas in png
    for i in range(2, arguments):
        print "Processing", str(argv[i])
        im = Image.open(str(argv[i])).convert('P')
        mask = np.array(im)
        indexed[mask > max_colour/2] = i
        #np.putmask(indexed, mask, mask/np.iinfo(mask.dtype).max*i)

        if(auxMap[2] != 0):
            #Rotamos e incrementamos
            auxMap = [auxMap[2]+50, 0, 0]
        else:
            auxMap = [0, auxMap[0], auxMap[1]]

        map_palette[i*3 : (i+1)*3] = auxMap

    imfile = Image.fromarray(indexed)
    imfile.putpalette(map_palette)
    imfile.save('pngIndexed.png')


if __name__ == "__main__":
    main(sys.argv)
