from pytermstreamxz import TermInflateStream, ByteInputStream
import numpy as np

def main():
    with open("recording1bit-CPU-2.texz", "rb") as file:
        bytes_list = file.read()
    byte_stream = ByteInputStream(list(bytes_list))
    inflate_stream = TermInflateStream(byte_stream)
    inflate_stream.seek(1025)
    for _ in range(65536):
        frame = inflate_stream.read_frame()

        width, height = frame.width, frame.height

        # Read and optionally transform codepoints
        codepoints_arr = np.zeros(width * height, dtype=np.uint32)

        # Read foreground RGB channels
        fg_red = np.zeros(width * height, dtype=np.uint8)
        fg_green = np.zeros(width * height, dtype=np.uint8)
        fg_blue = np.zeros(width * height, dtype=np.uint8)

        # Read background RGB channels
        bg_red = np.zeros(width * height, dtype=np.uint8)
        bg_green = np.zeros(width * height, dtype=np.uint8)
        bg_blue = np.zeros(width * height, dtype=np.uint8)

        frame.read_components(
            codepoints_out=codepoints_arr.ctypes.data,
            fg_r_out=fg_red.ctypes.data,
            fg_g_out=fg_green.ctypes.data,
            fg_b_out=fg_blue.ctypes.data,
            bg_r_out=bg_red.ctypes.data,
            bg_g_out=bg_green.ctypes.data,
            bg_b_out=bg_blue.ctypes.data,
            num_elements=width * height
        )

        for row in range(height):
            for col in range(width):
                idx = row * width + col
                codepoint = codepoints_arr[idx]
                print(chr(codepoint), end="")
            print()
        print("\n" + "=" * 40 + "\n")


if __name__ == "__main__":
    main()
