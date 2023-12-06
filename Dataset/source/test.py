import os
import numpy as np
import idx2numpy
from PIL import Image

def convert_images_to_idx(images_folder, output_idx3_filename):
    image_files = []
    labels = []
    for label_name in os.listdir(images_folder):
        label_folder = os.path.join(images_folder, label_name)
        if os.path.isdir(label_folder):
            for image_file in os.listdir(label_folder):
                image_files.append(os.path.join(label_folder, image_file))
                labels.append(int(label_name))

    images = np.array([np.array(Image.open(image_file)) for image_file in image_files], dtype=np.uint8)
    labels = np.array(labels, dtype=np.uint8)

    with open(output_idx3_filename, 'wb') as idx3_file:
        idx3_file.write(np.array([0, 0, 8, 3], dtype=np.uint8).tobytes())  # IDX3 magic number
        idx3_file.write(np.array(len(images), dtype='>i4').tobytes())  # Number of images
        idx3_file.write(np.array(images.shape[1:], dtype='>i4').tobytes())  # Dimensions of each image
        idx3_file.write(images.tobytes())

def convert_labels_to_idx(labels, output_idx1_filename):
    labels = np.array(labels, dtype=np.uint8)

    with open(output_idx1_filename, 'wb') as idx1_file:
        idx1_file.write(np.array([0, 0, 8, 1], dtype=np.uint8).tobytes())  # IDX1 magic number
        idx1_file.write(np.array(len(labels), dtype='>i4').tobytes())  # Number of labels
        idx1_file.write(labels.tobytes())

# Provide the path to your root folder and the desired output filenames
images_folder = '/home/wtsaichu/Dokumente/Arbeitsplatz/Dataset/Dataset/Dataset_minimized (Kopie 1)/'
output_idx3_filename = './images.idx3-ubyte'
output_idx1_filename = './labels.idx1-ubyte'

convert_images_to_idx(images_folder, output_idx3_filename)
convert_labels_to_idx(labels, output_idx1_filename)
