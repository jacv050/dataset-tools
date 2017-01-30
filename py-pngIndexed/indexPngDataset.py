from PIL import Image
import numpy as np
import sys
import csv

#Cambiar para que al introducir una ruta poder ponerle un apodo. en plan
#ruta/mascara.png:objecto_diccionario

def processDataset(dataset):
    dataset_dict = {}
    with open(dataset,'rb') as csvfile:
        spamreader = csv.reader(csvfile, delimiter=';', quotechar='|')
        for row in spamreader:
            colour = row[2].split(',')
            dataset_dict[row[0]] = (int(row[1]), [int(colour[0]),int(colour[1]),int(colour[2])])
            #print ' '.join(row)
    
    return dataset_dict

def main(argv):
    arguments = len(argv)
    if arguments <= 3:
        print "You must introduce:"
        print "\tscript.py dataset.csv output.png list_of_ masks_routes(python-script dataset mask1[:object_name] mask2[:object_name] mask3[:object_name]...)"
        sys.exit(0)    

    dataset = processDataset(argv[1])
    #print dataset

    #Quantity of masks
    quantyMasks = arguments - 3

    #Create double palette colour
    max_colour = np.iinfo('uint8').max
    map_palette = [0]*(max_colour+1)*3
    indexed = []
    pos_delimiter = str(argv[3]).find(':')
    if(pos_delimiter == -1):
        indexed = np.array(Image.open(str(argv[3])).convert('P'))*0
    else:
        indexed = np.array(Image.open(str(argv[3])[0:pos_delimiter]).convert('P'))*0

    #Indexing mas in png
    for i in range(3, arguments):
        print "Processing", argv[i]
        splitted = argv[i].split('/')
        name = splitted[len(splitted)-1]
        key = ""
        nameFile = ""
        pos_delimiter = name.find(':')
        if(pos_delimiter == -1):
            key = name.split('.')[0]
            nameFile = argv[i]
        else:
            key = name[pos_delimiter+1:len(name)]
            nameFile = argv[i].split(':')[0]
            #print key

        print key
        data = dataset[key]
        im = Image.open(nameFile).convert('P')
        mask = np.array(im)
        indexed[mask > max_colour/2] = data[0]
        map_palette[data[0]*3 : (data[0]+1)*3] = data[1]

    imfile = Image.fromarray(indexed)
    imfile.putpalette(map_palette)
    imfile.save(argv[2])


if __name__ == "__main__":
    main(sys.argv)
