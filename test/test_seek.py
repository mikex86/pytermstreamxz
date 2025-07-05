import time
from random import Random

from pytermstreamxz import TermInflateStream, ByteInputStream


def main():
    with open("recording1.texz", "rb") as file:
        bytes_list = file.read()
    byte_stream = ByteInputStream(list(bytes_list))
    inflate_stream = TermInflateStream(byte_stream)

    max_frame = inflate_stream.get_total_num_frames()

    rng = Random(123)
    seek_counter = 0
    start_time = time.perf_counter()
    while True:
        frame_idx = rng.randint(0, max_frame)
        inflate_stream.seek(frame_idx)
        frame = inflate_stream.read_frame()
        seek_counter += 1
        end_time = time.perf_counter()
        elapsed_time = end_time - start_time
        print(f"Seeks per second: {seek_counter / elapsed_time:.2f}")


if __name__ == "__main__":
    main()
