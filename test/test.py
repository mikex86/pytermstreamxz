from pytermstreamxz import TermInflateStream, ByteInputStream

def main():
    with open("repository.texz", "rb") as file:
        bytes_list = file.read()
    byte_stream = ByteInputStream(list(bytes_list))
    inflate_stream = TermInflateStream(byte_stream)

    while inflate_stream.has_next_frame():
        frame = inflate_stream.read_frame()

        for row in range(frame.height):
            for col in range(frame.width):
                cell = frame.get_cell(col, row)
                c = chr(cell.codepoint)
                print(c, end='')
            print()
        print("-")

if __name__ == "__main__":
    main()
