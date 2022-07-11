from typing import List
import zlib
import struct
from typing import BinaryIO
from PIL import Image
import random


BLACK_PIXEL = (0, 0, 0)
WHITE_PIXEL = (255, 255, 255)

HEADER = b'\x89PNG\r\n\x1A\n' #png文件头，固定格式

def generate_checkerboard_pattern(width: int, height: int) -> Image:
	out = []
	for i in range(height):
		# Generate a single row of white/black pixels
		row = []
		for j in range(width):
			if (i + j) % 8 == 0:
				row.append(BLACK_PIXEL)
			else:
				row.append(WHITE_PIXEL)
		out.append(row)
	return out

def get_checksum(chunk_type: bytes, data: bytes) -> int:
	checksum = zlib.crc32(chunk_type)
	checksum = zlib.crc32(data, checksum)
	return checksum

def chunk(out: BinaryIO, chunk_type: bytes, data: bytes) -> None:
	out.write(struct.pack('>I', len(data)))
	out.write(chunk_type)
	out.write(data)

	checksum = get_checksum(chunk_type, data)
	out.write(struct.pack('>I', checksum))
	
def make_ihdr(width: int, height: int, bit_depth: int, color_type: int) -> bytes:
	return struct.pack('>2I5B', width, height, bit_depth, color_type, 0, 0, 0)

def encode_data(img: Image) -> List[int]:
	ret = []

	for row in img:
		ret.append(0)

		color_values = [
			color_value
			for pixel in row
			for color_value in pixel
		]
		ret.extend(color_values)

	return ret

def compress_data(data: List[int]) -> bytes:
	data_bytes = bytearray(data)
	return zlib.compress(data_bytes)

def make_idat(img: Image) -> bytes:
	encoded_data = encode_data(img)
	compressed_data = compress_data(encoded_data)
	return compressed_data

def dump_png(out: BinaryIO, img: Image) -> None:
	out.write(HEADER)  # start by writing the header  #写头
	assert(len(img) > 0)  # assume we were not given empty image data
	width = len(img[0])  #图像的宽度
	height = len(img)    #图像的高度
	bit_depth = 8  # bits per pixel
	color_type = 2  # pixel is RGB triple
	
	ihdr_data = make_ihdr(width, height, bit_depth, color_type) #IHDR
	chunk(out, b'IHDR', ihdr_data)
	compressed_data = make_idat(img)
	chunk(out, b'IDAT', data=compressed_data)
	chunk(out, b'IEND', data=b'')
	
def save_png(img: Image, filename: str) -> None:
	with open(filename, 'wb') as out:
		dump_png(out, img)
        
width = 100
height = 100

img = generate_checkerboard_pattern(width, height)
save_png(img, 'generate_out.png')

