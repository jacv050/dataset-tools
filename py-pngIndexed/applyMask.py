from PIL import Image
import numpy as np
import sys

def main(argv):
    arguments = len(argv)
    output = "maskAplied.png"
    if arguments < 3 or arguments > 4:
        print "script origial_image binary_mask [outputimage=maskAplied.png]"
        sys.exit(0)

    if arguments == 4:
        output = argv[3]

    print("Processing...")
    ima = np.array(Image.open(str(argv[1])))
    maska = np.array(Image.open(str(argv[2])))
    outimage = ima

    for i in range(0, maska.shape[0]):
        for j in range(0, maska.shape[1]):
            if(maska[i,j] == 0):
                outimage[i,j]=(0,0,0)

    Image.fromarray(outimage).save(output)

if __name__ == "__main__":
    main(sys.argv)
