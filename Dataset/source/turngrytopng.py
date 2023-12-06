from PIL import Image
import numpy as np
import pandas as pd
def read_gry_file(filename, width, height):
    with open(filename, 'rb') as file:
        # Read the raw pixel values from the file
        pixels = np.fromfile(file, dtype=np.uint8, count=width*height)
        # Reshape the 1D array into a 2D array
        pixels = pixels.reshape((height, width))
    return pixels

def save_png_file(filename, pixels):
    # Convert the 2D array to a Pillow Image
    image = Image.fromarray(pixels, 'L')  # 'L' mode represents grayscale
    # Save the image as a PNG file
    image.save(filename, format='PNG')

def main():
    # Example dimensions of the image
    width = 640
    height = 240

    # Read the .gry file
    gry_filename = "/home/wtsaichu/Dokumente/Arbeitsplatz/Mikroprozessorsysteme-Abschluss/Dataset/test/test.gry"
    pixels = read_gry_file(gry_filename, width, height)
    print(pixels)
    print(type(pixels))
    # Save the pixels as a PNG file
    png_filename = 'output.png'
    save_png_file(png_filename, pixels)
    df = pd.DataFrame(pixels)

    # Specify the file path for the CSV file
    csv_filename = 'output.csv'

    # Export the DataFrame to a CSV file
    df.to_csv(csv_filename, index=False)
if __name__ == "__main__":
    main()
