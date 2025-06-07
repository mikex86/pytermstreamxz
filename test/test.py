from pytermstreamxz import TermInflateStream, ByteInputStream

def main():
    with open("repository.texz", "rb") as file:
        bytes_list = file.read()
    byte_stream = ByteInputStream(list(bytes_list))
    inflate_stream = TermInflateStream(byte_stream)

    while inflate_stream.has_next_frame():
        frame = inflate_stream.read_frame()
        print(f"Frame: {frame.width}x{frame.height}")


if __name__ == "__main__":
    main()
