from PIL import Image
import numpy as np

reference = np.array(Image.open("reference.ppm"))
width, height, pixels = reference.shape

errors = "ThinLensJittering \n"
for example in range(1, 45):
    experiment = np.array(Image.open("output-"+str(example)+".ppm"))
    sum = 0
    diff = (reference-experiment)/255
    sum += np.sum(np.sum(np.abs(diff)**2,axis=2)**(1./2))
    #for row in range(0, width):
    #    for column in range (0, height):
    #       sum += np.linalg.norm(diff[row, column])
    sum /= width * height
    print(sum)
    errors += str(example * example) + "," + str(sum) + "\n"
    
f = open("TLJ.csv", "w")
f.write(errors)
f.close()