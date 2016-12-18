from PIL import Image
import numpy as np
import sys

def main(argv):
    arguments = len(argv)
    output = ""
    if arguments < 2 | arguments > 3:
        print "sript png_indexed [outputroute=""]"
        sys.exit(0)

    if arguments == 3:
        output = argv[2]

    print "Processing", str(argv[1])
    im = Image.open(str(argv[1]))

    indexed = np.array(im)
    #Get the colour palette
    palette = im.getpalette()
    #Determine the total number of colours
    #num_colours = len(palette)/3
    #Dtermine maximum value of the image data type
    #max_val = float(np.iinfo(indexed.dtype).max)
    #Create a colour map matrix
    #map = np.array(palette).reshape(num_colours, 3) / max_val

    for i in range(1, np.amax(indexed)+1):
        mask = (i == indexed)*np.iinfo(indexed.dtype).max
        mask = np.array(mask, 'uint8')
        Image.fromarray(mask).save(output+"/mask"+str(i)+".png")

if __name__ == "__main__":
    main(sys.argv)
