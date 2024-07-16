from PIL import Image

ICOW, ICOH = 24, 24

def image_to_byte_array(image_path):
	image = Image.open(image_path).convert('L')
	image = image.resize((ICOW, ICOH))
	image = image.point(lambda p: p > 128 and 255)
	pixels = list(image.getdata())
	byte_array = []
	for pixel in pixels:
		byte_array.append(pixel)
	
	return byte_array

def format_byte_array(byte_array):
	formatted_str = '#define ICOW 24\n#define ICOH 24\n\nbyte_t default_icon[ICOW*ICOH] = {\n'
	for i in range(ICOW):
		line = '	' + ', '.join(map(str, byte_array[i*ICOH:(i+1)*ICOH])) + ',\n'
		formatted_str += line
	formatted_str += '};'
	return formatted_str

if __name__ == "__main__":
	image_path = 'default.png'
	
	byte_array = image_to_byte_array(image_path)
	formatted_str = format_byte_array(byte_array)
	
	print(formatted_str)
