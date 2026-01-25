from PIL import Image
import os

width = 16384
height = 16384

img = Image.new("RGB", (width, height), color=(12, 228, 128))

output = "test_16k.bmp"
img.save(output, format="BMP")

size_mb = os.path.getsize(output) / (1024 * 1024)
print(f"Arquivo: {width}x{height}")
print(f"Tamanho em disco: {size_mb:.2f} MB")
