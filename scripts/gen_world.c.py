#!/usr/bin/python3
from argparse import ArgumentParser


def main():
    argument_parser = ArgumentParser()
    argument_parser.add_argument("input_file")
    argument_parser.add_argument("output_file")
    argument_parser.add_argument("height_rows", type=int)
    argument_parser.add_argument("width_cols", type=int)
    argument_parser.add_argument("c_var_name")
    argument_parser.add_argument("c_constant_height")
    argument_parser.add_argument("c_constant_width")
    args = argument_parser.parse_args()
    with open(args.input_file) as ifp, open(args.output_file, "w") as ofp:
        print("/* This file is auto-generated. Do not edit manually! */", file=ofp)
        print('#include "world.h"', file=ofp)
        print(
            f"char {args.c_var_name}[{args.c_constant_height}][{args.c_constant_width}] = {{",
            file=ofp,
        )
        # Skip whitespace characters so I can format the world nicely while
        # editing it
        lines = ifp.readlines()
        n_non_space_lines = len([line for line in lines if not line.isspace()])
        if n_non_space_lines != args.height_rows:
            raise ValueError(f"World height is {args.height_rows} but got {len(lines)}")
        for i, line in enumerate(lines, 1):
            # Skip whitespace lines
            if line.isspace():
                continue

            n_non_space_cols = "".join(ch for ch in line if not ch.isspace())
            if len(n_non_space_cols) != args.width_cols:
                raise ValueError(
                    f"Max expected width is {args.width_cols} but line {i}'s width is {len(n_non_space_cols)}"
                )
            print(f'"{n_non_space_cols}"', end="", file=ofp)
            if len(lines) != i:
                print(",", file=ofp)
            else:
                print(file=ofp)
        print("};", file=ofp)


if __name__ == "__main__":
    main()
